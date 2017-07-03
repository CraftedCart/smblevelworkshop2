#include "resource/ResourceScene.hpp"
#include "GLManager.hpp"
#include "resource/ResourceManager.hpp"
#include <QByteArray>
#include <QFileInfo>
#include <QDebug>

namespace WS2 {
    namespace Resource {
        ResourceScene::ResourceScene() {}

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        ResourceScene::ResourceScene(QFile &file) {
            addModel(file);
        }

        QVector<ResourceMesh*>& ResourceScene::getMeshes() {
            return meshes;
        }

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        void ResourceScene::addModel(QFile &file) {
            addFilePath(file.fileName());
            QVector<ResourceMesh*> newMeshes = ResourceManager::addModel(file, isLoaded());
            meshes.append(newMeshes);
        }

       /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        void ResourceScene::load() {
            loaded = true;

            for (int i = 0; i < filePaths.size(); i++) {
                QFile f(filePaths.at(i));
                addModel(f);
            }
        }

        void ResourceScene::unload() {
            loaded = false;
        }
    }
}

