#include "ws2editor/resource/ResourceScene.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
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
            qDeleteAll(nodeMeshData.values());
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

        const Scene::SceneSelectionManager* ResourceScene::getSelectionManager() const {
            return selectionManager;
        }

        Physics::PhysicsManager* ResourceScene::getPhysicsManager() {
            return physicsManager;
        }

        void ResourceScene::addMeshNodeData(const WS2Common::Scene::SceneNode *node, MeshNodeData *data) {
            nodeMeshData[node] = data;

            physicsManager->addRigidBody(data->getPhysicsContainer()->getRigidBody());
        }

        bool ResourceScene::removeMeshNodeData(const WS2Common::Scene::SceneNode *node) {
            MeshNodeData *data = nodeMeshData.take(node);

            if (data != nullptr) {
                physicsManager->removeRigidBody(data->getPhysicsContainer()->getRigidBody());

                delete data;
                return true;
            } else {
                return false;
            }
        }

        MeshNodeData* ResourceScene::getMeshNodeData(const WS2Common::Scene::SceneNode *node) {
            return nodeMeshData[node];
        }

        const MeshNodeData* ResourceScene::getMeshNodeData(const WS2Common::Scene::SceneNode *node) const {
            return nodeMeshData[node];
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
                MeshSceneNode *meshNode = new MeshSceneNode(meshName);
                meshNode->setMeshName(meshName);

                UI::ModelManager::modelOutliner->addNodeWithMesh(meshNode, staticNode, meshes.at(i));
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

