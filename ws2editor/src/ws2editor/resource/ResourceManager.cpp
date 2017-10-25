#include "ws2editor/WS2.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/MathUtils.hpp"
#include "ws2editor/exception/IOException.hpp"
#include "ws2editor/exception/ModelLoadingException.hpp"
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <QDebug>
#include <climits>

namespace WS2Editor {
    namespace Resource {
        namespace ResourceManager {
            namespace ResourceManagerInternal {
                /**
                 * @throws WS2Editor::Exception::IOException When failing to read the file
                 * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
                 */
                QVector<ResourceMesh*> loadModel(QFile &file, bool shouldLoad) {
                    //The file is from elsewhere - Assume it's from the local filesystem, and pass it to Assimp
                    return addModelFromFile(file.fileName().toLatin1().constData(), shouldLoad);
                }

                /**
                 * @throws WS2Editor::Exception::ModelLoadingException When Assimp fails to generate an aiScene
                 */
                QVector<ResourceMesh*> addModelFromFile(const char *filePath, bool shouldLoad) {
                    Assimp::Importer importer;
                    const aiScene *scene = importer.ReadFile(
                            filePath,
                            aiProcess_Triangulate | aiProcess_GenNormals
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

                    QVector<Model::EditorMeshSegment*> segments; //Will contain all mesh segments for each material of this node's mesh
                    //Process this node's mesh segments
                    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                        Model::EditorMeshSegment *segment = processMeshSegment(mesh, scene, globalTransform, parentDir, shouldLoad);
                        segments.append(segment);
                    }

                    //Gather all segments of the mesh into a single ResourceMesh
                    if (segments.size() > 0) {
                        ResourceMesh *resMesh = new ResourceMesh();
                        resMesh->setId(node->mName.C_Str());
                        resMesh->setFilePath(*filePath);

                        for (int j = 0; j < segments.size(); j++) {
                            resMesh->addMeshSegment(segments.at(j));
                        }

                        if (shouldLoad) resMesh->load();

                        addResource(resMesh);
                        meshVector.append(resMesh);
                    }

                    //Recursively call this function to process meshes for all children
                    for (unsigned int i = 0; i < node->mNumChildren; i++) {
                        processNode(node->mChildren[i], scene, globalTransform, filePath, parentDir, meshVector, shouldLoad);
                    }
                }

                Model::EditorMeshSegment* processMeshSegment(
                        const aiMesh *mesh,
                        const aiScene *scene,
                        const glm::mat4 globalTransform,
                        const QDir *parentDir,
                        bool shouldLoad
                        ) {
                    QVector<WS2Common::Model::Vertex> vertices;
                    QVector<unsigned int> indices;
                    QVector<ResourceEditorTexture*> textures;

                    static const int UV_CHANNEL = 0;

                    //Just for the warning
                    if (!mesh->HasTextureCoords(UV_CHANNEL)) {
                        qWarning() << "A mesh is missing texuture coordinates - Defaulting to 0, 0 for all vertices";
                    }

                    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                        WS2Common::Model::Vertex vertex;
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
                    QVector<Resource::ResourceEditorTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, parentDir, shouldLoad);
                    textures.append(diffuseMaps);
                    QVector<Resource::ResourceEditorTexture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, parentDir, shouldLoad);
                    textures.append(specularMaps);

                    Model::EditorMeshSegment *segment = new Model::EditorMeshSegment(vertices, indices, textures);

                    return segment;
                }

                QVector<Resource::ResourceEditorTexture*> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const QDir *parentDir, bool shouldLoad) {
                    QVector<Resource::ResourceEditorTexture*> textures;

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

                        //Check if the file doesn't exist, and load the uvtemplate texture instead
                        if (!QFileInfo(filePath).exists()) {
                            filePath = ":/Workshop2/Images/uvtemplate.png";
                        }

                        //Don't load another copy of the texture if it is already in the ResourceManager
                        Resource::ResourceEditorTexture *texture = Resource::ResourceManager::getResourceFromFilePath<Resource::ResourceEditorTexture*>(filePath);

                        if (texture == nullptr) {
                            texture = new Resource::ResourceEditorTexture();
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

            QVector<WS2Common::Resource::AbstractResource*>& getResources() {
                static QVector<WS2Common::Resource::AbstractResource*> resources;
                return resources;
            }

            void addResource(WS2Common::Resource::AbstractResource *res) {
                getResources().append(res);
                if (qAppRunning) UI::ModelManager::modelResources->onResourceAdded();
            }

            void unloadAllResources() {
                for (int i = 0; i < getResources().size(); i++) {
                    WS2Common::Resource::AbstractResource *res = getResources().at(i);
                    if (res->isLoaded()) {
                        qDebug() << "Unloading:" << res->getId();
                        res->unload();
                    }
                }
            }

            /**
             * @throws WS2Editor::Exception::IOException When failing to read the file
             * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
             */
            QVector<ResourceMesh*> addModel(QFile &file, bool shouldLoad) {
                return ResourceManagerInternal::loadModel(file, shouldLoad);
            }

            /**
             * @throws WS2Editor::Exception::RuntimeException When there are UINT_MAX taken names
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

