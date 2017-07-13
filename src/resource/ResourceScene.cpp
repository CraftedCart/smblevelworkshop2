#include "resource/ResourceScene.hpp"
#include "GLManager.hpp"
#include "resource/ResourceManager.hpp"
#include "scene/MeshSceneNode.hpp"
#include <QByteArray>
#include <QFileInfo>
#include <QDebug>

namespace WS2 {
    namespace Resource {
        ResourceScene::ResourceScene() {
            rootNode = new Scene::SceneNode("root");
        }

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        ResourceScene::ResourceScene(QFile &file) {
            addModel(file);
        }

        Scene::SceneNode* ResourceScene::getRootNode() {
            return rootNode;
        }

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        void ResourceScene::addModel(QFile &file) {
            addFilePath(file.fileName());
            QVector<ResourceMesh*> newMeshes = ResourceManager::addModel(file, isLoaded());

            for (int i = 0; i < newMeshes.size(); i++) {
                //The .split("@")[0] gets the part of the name before the @ symbol, which should be the name of the mesh
                //TODO: Make ResourceMesh store the name of a mesh, instead of doing string manip to get the name
                Scene::MeshSceneNode *meshNode = new Scene::MeshSceneNode(newMeshes.at(i)->getId().split("@")[0], newMeshes.at(i));
                rootNode->addChild(meshNode);
            }
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

