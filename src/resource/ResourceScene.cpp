#include "resource/ResourceScene.hpp"
#include "GLManager.hpp"
#include "resource/ResourceManager.hpp"
#include "scene/MeshSceneNode.hpp"
#include "ui/ModelManager.hpp"
#include <QByteArray>
#include <QFileInfo>

namespace WS2 {
    namespace Resource {
        ResourceScene::ResourceScene() {
            rootNode = new Scene::SceneNode("root");
            rootNode->addChild(new Scene::SceneNode(tr("Static")));

            selectionManager = new Scene::SceneSelectionManager();
            connect(selectionManager, &Scene::SceneSelectionManager::onSelectionChanged,
                    this, &ResourceScene::onSelectionChanged);

            physicsManager = new Physics::PhysicsManager();
        }

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        ResourceScene::ResourceScene(QFile &file) {
            addModel(file);
        }

        ResourceScene::~ResourceScene() {
            delete rootNode;
            delete selectionManager;
            delete physicsManager;
            if (physicsDebugDrawer != nullptr) delete physicsDebugDrawer;
        }

        void ResourceScene::initPhysicsDebugDrawer() {
            physicsDebugDrawer = new PhysicsDebugDrawer();
            physicsDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
            physicsManager->getDynamicsWorld()->setDebugDrawer(physicsDebugDrawer);
        }

        PhysicsDebugDrawer* ResourceScene::getPhysicsDebugDrawer() {
            return physicsDebugDrawer;
        }

        Scene::SceneNode* ResourceScene::getRootNode() {
            return rootNode;
        }

        Scene::SceneSelectionManager* ResourceScene::getSelectionManager() {
            return selectionManager;
        }

        Physics::PhysicsManager* ResourceScene::getPhysicsManager() {
            return physicsManager;
        }

        /**
         * @throws WS2::Exception::IOException When failing to read the file
         * @throws WS2::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        void ResourceScene::addModel(QFile &file) {
            addFilePath(file.fileName());
            QVector<ResourceMesh*> newMeshes = ResourceManager::addModel(file, isLoaded());

            //Get the static node
            Scene::SceneNode *staticNode = rootNode->getChildByName(tr("Static"));
            //Create the static node if it doesn't exist
            if (!staticNode) {
                staticNode = new Scene::SceneNode(tr("Static"));
                rootNode->addChild(staticNode);
            }

            for (int i = 0; i < newMeshes.size(); i++) {
                //The .split("@")[0] gets the part of the name before the @ symbol, which should be the name of the mesh
                //TODO: Make ResourceMesh store the name of a mesh, instead of doing string manip to get the name
                Scene::MeshSceneNode *meshNode = new Scene::MeshSceneNode(newMeshes.at(i)->getId().split("@")[0], newMeshes.at(i));
                physicsManager->addRigidBody(meshNode->getPhysicsRigidBody());
                staticNode->addChild(meshNode);
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

        void ResourceScene::onSelectionChanged(QVector<Scene::SceneNode*>& selectedObjects) {
            UI::ModelManager::modelOutliner->selectionChanged(selectedObjects);
        }
    }
}

