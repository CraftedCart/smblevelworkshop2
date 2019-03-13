#include "ws2common/model/ModelLoader.hpp"
#include "ws2common/exception/IOException.hpp"
#include "ws2common/exception/ModelLoadingException.hpp"
#include "ws2common/MathUtils.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <QDebug>

namespace WS2Common {
    namespace Model {
        namespace ModelLoader {
                QVector<WS2Common::Resource::ResourceMesh*> loadModel(
                        QFile &file,
                        QVector<Resource::AbstractResource*> *resources,
                        QMutex *resourcesMutex
                        ) {
                    QString fileName = file.fileName();

                    if (fileName.startsWith(":")) {
                        //Is's from the resources - we must load binary data outselves
                        if (!file.open(QIODevice::ReadOnly)) {
                            throw WS2Common::Exception::IOException("Failed to open the resource file for reading");
                        }

                        QByteArray bytes = file.readAll();
                        return addModelFromMemory(bytes.data(), bytes.size(), resources, resourcesMutex);
                    } else {
                        //The file is from elsewhere - Assume it's from the local filesystem, and pass it to Assimp
                        return addModelFromFile(fileName.toLatin1().constData(), resources, resourcesMutex);
                    }
                }

                QVector<WS2Common::Resource::ResourceMesh*> addModelFromFile(
                        const char *filePath,
                        QVector<Resource::AbstractResource*> *resources,
                        QMutex *resourcesMutex
                        ) {
                    Assimp::Importer importer;
                    const aiScene *scene = importer.ReadFile(
                            filePath,
                            aiProcess_Triangulate | aiProcess_GenNormals
                            );

                    //Check if stuff went wrong
                    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                        //Oh noes!
                        throw WS2Common::Exception::ModelLoadingException(QString(importer.GetErrorString()));
                    }

                    const QFileInfo fileInfo = QFileInfo(filePath);
                    const QDir parentDir = fileInfo.dir();

                    QVector<WS2Common::Resource::ResourceMesh*> meshVector;

                    const glm::mat4 globalTransform = MathUtils::toGlmMat4(scene->mRootNode->mTransformation);
                    const QString filePathStr(filePath);
                    processNode(scene->mRootNode, scene, globalTransform, &filePathStr, &parentDir, meshVector, resources, resourcesMutex);

                    return meshVector;
                }

                QVector<WS2Common::Resource::ResourceMesh*> addModelFromMemory(
                        const void *bytes,
                        size_t byteCount,
                        QVector<Resource::AbstractResource*> *resources,
                        QMutex *resourcesMutex
                        ) {
                    Assimp::Importer importer;
                    const aiScene *scene = importer.ReadFileFromMemory(
                            bytes,
                            byteCount,
                            aiProcess_Triangulate | aiProcess_GenNormals
                            );

                    //Check if stuff went wrong
                    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                        //Oh noes!
                        throw WS2Common::Exception::ModelLoadingException(QString(importer.GetErrorString()));
                    }

                    QVector<WS2Common::Resource::ResourceMesh*> meshVector;

                    const glm::mat4 globalTransform = MathUtils::toGlmMat4(scene->mRootNode->mTransformation);
                    QString filePath;
                    QDir parentDir;
                    processNode(scene->mRootNode, scene, globalTransform, &filePath, &parentDir, meshVector, resources, resourcesMutex);

                    return meshVector;
                }

                void processNode(
                        const aiNode *node,
                        const aiScene *scene,
                        const glm::mat4 globalTransform,
                        const QString *filePath,
                        const QDir *parentDir,
                        QVector<WS2Common::Resource::ResourceMesh*> &meshVector,
                        QVector<Resource::AbstractResource*> *resources,
                        QMutex *resourcesMutex
                        ) {
                    //qInfo() << "Processing node" << node->mName.C_Str() << node->mNumMeshes;

                    QVector<WS2Common::Model::MeshSegment*> segments; //Will contain all mesh segments for each material of this node's mesh
                    //Process this node's mesh segments
                    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                        WS2Common::Model::MeshSegment *segment = processMeshSegment(mesh, scene, globalTransform, parentDir, resources, resourcesMutex);
                        segments.append(segment);
                    }

                    //Gather all segments of the mesh into a single WS2Common::Resource::ResourceMesh
                    if (segments.size() > 0) {
                        WS2Common::Resource::ResourceMesh *resMesh = new WS2Common::Resource::ResourceMesh();
                        resMesh->setId(node->mName.C_Str());
                        resMesh->setFilePath(*filePath);

                        for (int j = 0; j < segments.size(); j++) {
                            resMesh->addMeshSegment(segments.at(j));
                        }

                        if (resources != nullptr) {
                            if (resourcesMutex != nullptr) resourcesMutex->lock();
                            resources->append(resMesh);
                            if (resourcesMutex != nullptr) resourcesMutex->unlock();
                        }

                        meshVector.append(resMesh);
                    }

                    //Recursively call this function to process meshes for all children
                    for (unsigned int i = 0; i < node->mNumChildren; i++) {
                        processNode(node->mChildren[i], scene, globalTransform, filePath, parentDir, meshVector, resources, resourcesMutex);
                    }
                }

                WS2Common::Model::MeshSegment* processMeshSegment(
                        const aiMesh *mesh,
                        const aiScene *scene,
                        const glm::mat4 globalTransform,
                        const QDir *parentDir,
                        QVector<Resource::AbstractResource*> *resources,
                        QMutex *resourcesMutex
                        ) {
                    QVector<WS2Common::Model::Vertex> vertices;
                    QVector<unsigned int> indices;
                    QVector<WS2Common::Resource::ResourceTexture*> textures;

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
                    QVector<WS2Common::Resource::ResourceTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, parentDir, resources, resourcesMutex);
                    textures.append(diffuseMaps);
                    QVector<WS2Common::Resource::ResourceTexture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, parentDir, resources, resourcesMutex);
                    textures.append(specularMaps);

                    WS2Common::Model::MeshSegment *segment = new WS2Common::Model::MeshSegment(vertices, indices, textures);

                    return segment;
                }

                QVector<WS2Common::Resource::ResourceTexture*> loadMaterialTextures(
                        aiMaterial *mat,
                        aiTextureType type,
                        const QDir *parentDir,
                        QVector<Resource::AbstractResource*> *resources,
                        QMutex *resourcesMutex
                        ) {
                    QVector<WS2Common::Resource::ResourceTexture*> textures;

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
                            filePath = ":/WS2Common/Images/uvtemplate.png";
                        }

                        WS2Common::Resource::ResourceTexture *texture = nullptr;
                        //Don't load another copy of the texture if it is already in the ResourceManager
                        if (resources != nullptr) {
                            texture = getResourceFromFilePath<WS2Common::Resource::ResourceTexture*>(filePath, *resources, resourcesMutex);
                        }

                        if (texture == nullptr) {
                            texture = new WS2Common::Resource::ResourceTexture();
                            texture->setId(filePath);
                            texture->setFilePath(filePath);
                            if (resources != nullptr) {
                                if (resourcesMutex != nullptr) resourcesMutex->lock();
                                resources->append(texture);
                                if (resourcesMutex != nullptr) resourcesMutex->unlock();
                            }
                        }

                        if (!textures.contains(texture)) textures.append(texture);
                    }

                    return textures;
                }
        }
    }
}

