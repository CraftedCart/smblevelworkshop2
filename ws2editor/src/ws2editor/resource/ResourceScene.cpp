#include "ws2editor/resource/ResourceScene.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include <QByteArray>
#include <QFileInfo>

namespace WS2Editor {
    namespace Resource {
        ResourceScene::ResourceScene() {
            rootNode = new WS2Common::Scene::SceneNode("root");
            WS2Common::Scene::GroupSceneNode *staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
            UI::ModelManager::modelOutliner->addNode(staticNode, rootNode);

            selectionManager = new Scene::SceneSelectionManager();
            connect(selectionManager, &Scene::SceneSelectionManager::onSelectionChanged,
                    this, &ResourceScene::onSelectionChanged);

            physicsManager = new Physics::PhysicsManager();
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

        WS2Common::Scene::SceneNode* ResourceScene::getStaticNode() {
            using namespace WS2Common::Scene;

            //Search for non-animated nodes
            for (SceneNode *node : rootNode->getChildren()) {
                if (node->getTransformAnimation() == nullptr) return node;
            }

            //Nothing found
            return nullptr;
        }

        Scene::SceneSelectionManager* ResourceScene::getSelectionManager() {
            return selectionManager;
        }

        Physics::PhysicsManager* ResourceScene::getPhysicsManager() {
            return physicsManager;
        }

        /**
         * @throws WS2Editor::Exception::IOException When failing to read the file
         */
        void ResourceScene::addModel(const QVector<WS2Common::Resource::ResourceMesh*> &meshes) {
            using namespace WS2Common::Scene;
            using namespace WS2Editor::Scene;

            //Get the static node
            WS2Common::Scene::SceneNode *staticNode = rootNode->getChildByName(tr("Static"));
            //Create the static node if it doesn't exist
            if (!staticNode) {
                staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
                UI::ModelManager::modelOutliner->addNode(staticNode, rootNode);
            }

            for (int i = 0; i < meshes.size(); i++) {
                //The .split("@")[0] gets the part of the name before the @ symbol, which should be the name of the mesh
                //TODO: Make ResourceMesh store the name of a mesh, instead of doing string manip to get the name
                QString meshName = meshes.at(i)->getId().split("@")[0];
                EditorMeshSceneNode *meshNode = new EditorMeshSceneNode(meshName, this, meshes.at(i));
                meshNode->setMeshName(meshName);

                UI::ModelManager::modelOutliner->addNode(meshNode, staticNode);
            }
        }

        void ResourceScene::load() {
            loaded = true;
        }

        void ResourceScene::unload() {
            loaded = false;
        }

        void ResourceScene::onSelectionChanged(QVector<WS2Common::Scene::SceneNode*>& selectedObjects) {
            UI::ModelManager::modelOutliner->selectionChanged(selectedObjects);
        }
    }
}

