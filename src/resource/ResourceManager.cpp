#include "WS2.hpp"
#include "resource/ResourceManager.hpp"
#include "ui/ModelManager.hpp"
#include "MathUtils.hpp"
#include "exception/IOException.hpp"
#include "exception/ModelLoadingException.hpp"
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <QDebug>
#include <climits>

namespace WS2 {
    namespace Resource {
        namespace ResourceManager {
            namespace ResourceManagerInternal {
                /**
                 * @throws WS2::Exception::IOException When failing to read the file
                 * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
                 */
                QVector<ResourceMesh*> loadModel(QFile &file, bool shouldLoad) {
                    //The file is from elsewhere - Assume it's from the local filesystem, and pass it to Assimp
                    return addModelFromFile(file.fileName().toLatin1().constData(), shouldLoad);
                }

                /**
                 * @throws WS2::Exception::ModelLoadingException When Assimp fails to generate an aiScene
                 */
                QVector<ResourceMesh*> addModelFromFile(const char *filePath, bool shouldLoad) {
                    Assimp::Importer importer;
                    const aiScene *scene = importer.ReadFile(
                            filePath,
                            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals
                            );

                    //Check if stuff went wrong
                    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                        //Oh noes!
                        throw Exception::ModelLoadingException(QString(importer.GetErrorString()));
                    }

                    const QFileInfo fileInfo = QFileInfo(filePath);
                    const QDir parentDir = fileInfo.dir();

                    QVector<ResourceMesh*> meshVector;

                    const glm::mat4 globalTransform = MathUtils::toGlmMat4(scene->mRootNode->mTransformation);
                    const QString filePathStr(filePath);
                    processNode(scene->mRootNode, scene, globalTransform, &filePathStr, &parentDir, meshVector, shouldLoad);

                    return meshVector;
                }

                void processNode(
                        const aiNode *node,
                        const aiScene *scene,
                        const glm::mat4 globalTransform,
                        const QString *filePath,
                        const QDir *parentDir,
                        QVector<ResourceMesh*> &meshVector,
                        bool shouldLoad
                        ) {
                    qDebug() << "Processing node" << node->mName.C_Str() << node->mNumMeshes;

                    //Process this node's messages
                    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                        ResourceMesh *resMesh = processMesh(mesh, scene, globalTransform, filePath, parentDir, shouldLoad);
                        addResource(resMesh);
                        meshVector.append(resMesh);
                    }

                    //Recursively call this function to process meshes for all children
                    for (unsigned int i = 0; i < node->mNumChildren; i++) {
                        processNode(node->mChildren[i], scene, globalTransform, filePath, parentDir, meshVector, shouldLoad);
                    }
                }

                ResourceMesh* processMesh(
                        const aiMesh *mesh,
                        const aiScene *scene,
                        const glm::mat4 globalTransform,
                        const QString *filePath,
                        const QDir *parentDir,
                        bool shouldLoad
                        ) {
                    QVector<Model::Vertex> vertices;
                    QVector<unsigned int> indices;
                    QVector<ResourceTexture*> textures;

                    static const int UV_CHANNEL = 0;

                    //Just for the warning
                    if (!mesh->HasTextureCoords(UV_CHANNEL)) {
                        qDebug() << "Warning: A mesh is missing texuture coordinates - Defaulting to 0, 0 for all vertices";
                    }

                    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                        Model::Vertex vertex;
                        glm::vec3 vec;

                        //Process vertex positions, normals, and texture coordinates
                        vec.x = mesh->mVertices[i].x;
                        vec.y = mesh->mVertices[i].y;
                        vec.z = mesh->mVertices[i].z;
                        vec = glm::vec3(glm::vec4(vec, 1.0f) / globalTransform);
                        vertex.position = vec;

                        //mNormals can be a nullptr sometimes
                        //For example, when trying to import an OBJ that has a curve exported from Blender
                        if (mesh->mNormals != nullptr) {
                            vec.x = mesh->mNormals[i].x;
                            vec.y = mesh->mNormals[i].y;
                            vec.z = mesh->mNormals[i].z;
                            vertex.normal = vec;
                        }

                        if (mesh->mTextureCoords[UV_CHANNEL]) {
                            glm::vec2 vec2;
                            vec2.x = mesh->mTextureCoords[UV_CHANNEL][i].x;
                            vec2.y = mesh->mTextureCoords[UV_CHANNEL][i].y;

                            vertex.texCoord = vec2;
                        } else {
                            vertex.texCoord = glm::vec2(0.0f, 0.0f);
                        }

                        vertices.append(vertex);
                    }

                    //Process indices (Faces vertices)
                    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                        aiFace face = mesh->mFaces[i];
                        for (unsigned int j = 0; j < face.mNumIndices; j++) {
                            indices.append(face.mIndices[j]);
                        }
                    }

                    //Process material
                    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
                    QVector<Resource::ResourceTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, parentDir, shouldLoad);
                    textures.append(diffuseMaps);
                    QVector<Resource::ResourceTexture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, parentDir, shouldLoad);
                    textures.append(specularMaps);

                    ResourceMesh *resMesh = new ResourceMesh(vertices, indices, textures);
                    resMesh->setId(QString("%1@%2").arg(mesh->mName.C_Str()).arg(*filePath));
                    resMesh->setFilePath(*filePath);
                    resMesh->load();

                    return resMesh;
                }

                QVector<Resource::ResourceTexture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const QDir *parentDir, bool shouldLoad) {
                    QVector<Resource::ResourceTexture*> textures;

                    int maxI = mat->GetTextureCount(type);
                    for (int i = 0; i < maxI; i++) {
                        aiString str;
                        mat->GetTexture(type, i, &str);
                        const QFileInfo fileInfo = QFileInfo(str.C_Str());
                        QString filePath;
                        if ((fileInfo.isRelative() || QString(str.C_Str()).startsWith("//")) && parentDir != nullptr) {
                            //The filepath is relative - Make it absolute
                            //Filepaths that begin with // are relative (Due to Blender)
                            QString relPath = str.C_Str();
                            if (relPath.startsWith("//")) relPath.remove(0, 2);

                            filePath = parentDir->absoluteFilePath(relPath);
                        } else {
                            //The filepath is absolute or parentDir was nullptr
                            filePath = QString(str.C_Str());
                        }

                        //Don't load another copy of the texture if it is already in the ResourceManager
                        Resource::ResourceTexture *texture = Resource::ResourceManager::getResourceFromFilePath<Resource::ResourceTexture*>(filePath);

                        if (texture == nullptr) {
                            texture = new Resource::ResourceTexture();
                            texture->setId(filePath);
                            texture->setFilePath(filePath);
                            if (shouldLoad) texture->load();
                            addResource(texture);
                        } else if (!texture->isLoaded() && shouldLoad) {
                            //Load the texture if one is found but not loaded, and if it should be loaded
                            texture->load();
                        }

                        if (!textures.contains(texture)) textures.append(texture);
                    }

                    return textures;
                }
            } //End of ResourceManagerInternal

            QVector<AbstractResource*>& getResources() {
                static QVector<AbstractResource*> resources;
                return resources;
            }

            void addResource(AbstractResource *res) {
                getResources().append(res);
                if (qAppRunning) UI::ModelManager::modelResources->onResourceAdded();
            }

            void unloadAllResources() {
                for (int i = 0; i < getResources().size(); i++) {
                    AbstractResource *res = getResources().at(i);
                    if (res->isLoaded()) {
                        qDebug() << "Unloading:" << res->getId();
                        res->unload();
                    }
                }
            }

            /**
             * @throws WS2::Exception::IOException When failing to read the file
             * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
             */
            QVector<ResourceMesh*> addModel(QFile &file, bool shouldLoad) {
                return ResourceManagerInternal::loadModel(file, shouldLoad);
            }

            /**
             * @throws WS2::Exception::RuntimeException When there are UINT_MAX taken names
             */
            QString generateUniqueId(QString prefix) {
                //Create and populate vector of taken names
                int size = getResources().size();
                QVector<QString> takenNames(size);

                for (int i = 0; i < size; i++) {
                    takenNames[i] = getResources().at(i)->getId();
                }

                //Keep generating a new Id with an increacing suffix number until one isn't taken
                unsigned int suffix = 0;
                QString str; //No need to allocate str on each iteration of the loop

                while (suffix < UINT_MAX) {
                    str = QString("%1%2").arg(prefix).arg(suffix);
                    if (!takenNames.contains(str)) return str;

                    suffix++;
                }

                //Ok so someone had the idea of having UINT_MAX taken names
                throw Exception::RuntimeException("There are UINT_MAX or more matching names in "
                        "generateUniqueId(QString prefix)");
            }
        }
    }
}

