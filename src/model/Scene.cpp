#include "model/Scene.hpp"
#include "MathUtils.hpp"
#include "GLManager.hpp"
#include "exception/IOException.hpp"
#include "exception/ModelLoadingException.hpp"
#include <QByteArray>
#include <QFileInfo>
#include <QDebug>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace WS2 {
    namespace Model {

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        Scene::Scene(QFile &file) {
            addModel(file);
        }

        QVector<Mesh>& Scene::getMeshes() {
            return meshes;
        }

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        void Scene::addModel(QFile &file) {
            if (file.fileName().startsWith(":")) {
                //The file is from the resources - Read the whole file and give the data to Assimp

                if (!file.open(QFile::ReadOnly)) {
                    throw Exception::IOException(file.errorString());
                }

                QByteArray bytes = file.readAll();
                file.close();

                QFileInfo fileInfo = QFileInfo(file);
                addModelFromMemory(bytes, fileInfo.suffix().toLatin1().constData());
            } else {
                //The file is from elsewhere - Assume it's from the local filesystem, and pass it to Assimp
                addModelFromFile(file.fileName().toLatin1().constData());
            }

        }

        /**
         * @throws WS2::Exception::ModelLoadingException When Assimp fails to generate an aiScene
         */
        void Scene::addModelFromMemory(const QByteArray &bytes, const char *hint) {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFileFromMemory(
                    bytes.constData(),
                    bytes.size(),
                    aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals,
                    hint
                    );

            //Check if stuff went wrong
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                //Oh noes!
                throw Exception::ModelLoadingException(QString(importer.GetErrorString()));
            }

            const glm::mat4 globalTransform = MathUtils::toGlmMat4(scene->mRootNode->mTransformation);
            processNode(scene->mRootNode, scene, globalTransform, nullptr);
        }

        /**
         * @throws WS2::Exception::ModelLoadingException When Assimp fails to generate an aiScene
         */
        void Scene::addModelFromFile(const char *filePath) {
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

            const glm::mat4 globalTransform = MathUtils::toGlmMat4(scene->mRootNode->mTransformation);
            processNode(scene->mRootNode, scene, globalTransform, &parentDir);
        }

        void Scene::processNode(const aiNode *node, const aiScene *scene, const glm::mat4 globalTransform, const QDir *parentDir) {
            qDebug() << "Processing node" << node->mName.C_Str();

            //Process this node's messages
            for (unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.append(processMesh(mesh, scene, globalTransform, parentDir));
            }

            //Recursively call this function to process meshes for all children
            for (unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene, globalTransform, parentDir);
            }
        }

        Mesh Scene::processMesh(const aiMesh *mesh, const aiScene *scene, const glm::mat4 globalTransform, const QDir *parentDir) {
            QVector<Vertex> vertices;
            QVector<unsigned int> indices;
            QVector<Resource::ResourceTexture*> textures;

            static const int UV_CHANNEL = 0;

            //Just for the warning
            if (!mesh->HasTextureCoords(UV_CHANNEL)) {
                qDebug() << "Warning: A mesh is missing texuture coordinates - Defaulting to 0, 0 for all vertices";
            }

            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                Vertex vertex;
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
            QVector<Resource::ResourceTexture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, parentDir);
            textures.append(diffuseMaps);
            QVector<Resource::ResourceTexture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, parentDir);
            textures.append(specularMaps);

            return Mesh(vertices, indices, textures);
        }

        QVector<Resource::ResourceTexture*> Scene::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const QDir *parentDir) {
            QVector<Resource::ResourceTexture*> textures;

            int maxI = mat->GetTextureCount(type);
            for (unsigned int i = 0; i < maxI; i++) {
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

                Resource::ResourceTexture *texture = new Resource::ResourceTexture();
                texture->setFilePath(filePath);
                texture->load();

                textures.append(texture);
            }

            return textures;
        }
    }
}

