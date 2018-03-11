#include "ws2editor/resource/ResourceScene.hpp"
#include "ws2editor/GLManager.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QByteArray>
#include <QFileInfo>

namespace WS2Editor {
    namespace Resource {
        ResourceScene::ResourceScene() {
            rootNode = new WS2Common::Scene::SceneNode("root");
            WS2Common::Scene::GroupSceneNode *staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
            rootNode->addChild(staticNode);
            UI::ModelManager::modelOutliner->onNodeAdded(staticNode); //TODO: This feels hacky

            selectionManager = new Scene::SceneSelectionManager();
            connect(selectionManager, &Scene::SceneSelectionManager::onSelectionChanged,
                    this, &ResourceScene::onSelectionChanged);

            physicsManager = new Physics::PhysicsManager();
        }

        /**
         * @throws WS2Editor::Exception::IOException When failing to read the file
         * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
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

        WS2Common::Scene::SceneNode* ResourceScene::getRootNode() {
            return rootNode;
        }

        Scene::SceneSelectionManager* ResourceScene::getSelectionManager() {
            return selectionManager;
        }

        Physics::PhysicsManager* ResourceScene::getPhysicsManager() {
            return physicsManager;
        }

        /**
         * @throws WS2Editor::Exception::IOException When failing to read the file
         * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
         */
        void ResourceScene::addModel(QFile &file) {
            using namespace WS2Common::Resource;
            using namespace WS2Common::Scene;
            using namespace WS2Editor::Scene;

            addFilePath(file.fileName());
            QVector<ResourceMesh*> newMeshes = ResourceManager::addModel(file, isLoaded());

            //Get the static node
            WS2Common::Scene::SceneNode *staticNode = rootNode->getChildByName(tr("Static"));
            //Create the static node if it doesn't exist
            if (!staticNode) {
                staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
                rootNode->addChild(staticNode);
                UI::ModelManager::modelOutliner->onNodeAdded(staticNode); //TODO: This feels hacky
            }

            for (int i = 0; i < newMeshes.size(); i++) {
                //The .split("@")[0] gets the part of the name before the @ symbol, which should be the name of the mesh
                //TODO: Make ResourceMesh store the name of a mesh, instead of doing string manip to get the name
                QString meshName = newMeshes.at(i)->getId().split("@")[0];
                EditorMeshSceneNode *meshNode = new EditorMeshSceneNode(meshName, newMeshes.at(i));
                meshNode->setMeshName(meshName);
                physicsManager->addRigidBody(meshNode->getPhysicsRigidBody());
                staticNode->addChild(meshNode);
                UI::ModelManager::modelOutliner->onNodeAdded(meshNode); //TODO: This feels hacky
            }
        }

        /**
         * @throws WS2Editor::Exception::IOException When failing to read the file
         * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
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

        void ResourceScene::onSelectionChanged(QVector<WS2Common::Scene::SceneNode*>& selectedObjects) {
            UI::ModelManager::modelOutliner->selectionChanged(selectedObjects);
        }
    }
}

