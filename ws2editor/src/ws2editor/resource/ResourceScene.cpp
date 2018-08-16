#include "ws2editor/resource/ResourceScene.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/scene/MeshCollisionSceneNode.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include <QByteArray>
#include <QFileInfo>

namespace WS2Editor {
    namespace Resource {
        using namespace WS2Common::Resource;
        using namespace WS2Common;

        ResourceScene::ResourceScene() {
            stage = new Stage();
            stage->setRootNode(new WS2Common::Scene::SceneNode("root"));
            stage->setFalloutY(-10.0f); //Default fallout plane

            WS2Common::Scene::GroupSceneNode *staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
            UI::ModelManager::modelOutliner->addNode(staticNode, stage->getRootNode());

            selectionManager = new Scene::SceneSelectionManager();
            connect(selectionManager, &Scene::SceneSelectionManager::onSelectionChanged,
                    this, &ResourceScene::onSelectionChanged);

            physicsManager = new Physics::PhysicsManager();
        }

        ResourceScene::~ResourceScene() {
            delete stage;
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

        Stage* ResourceScene::getStage() {
            return stage;
        }

        const Stage* ResourceScene::getStage() const {
            return stage;
        }

        WS2Common::Scene::SceneNode* ResourceScene::getRootNode() {
            return stage->getRootNode();
        }

        WS2Common::Scene::SceneNode* ResourceScene::getStaticNode() {
            using namespace WS2Common::Scene;

            //Search for non-animated nodes
            for (SceneNode *node : stage->getRootNode()->getChildren()) {
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

        void ResourceScene::addMeshNodeData(const QUuid &uuid, MeshNodeData *data) {
            nodeMeshData[uuid] = data;

            physicsManager->addRigidBody(data->getPhysicsContainer()->getRigidBody());
        }

        bool ResourceScene::removeMeshNodeData(const QUuid &uuid) {
            MeshNodeData *data = nodeMeshData.take(uuid);

            if (data != nullptr) {
                physicsManager->removeRigidBody(data->getPhysicsContainer()->getRigidBody());

                delete data;
                return true;
            } else {
                return false;
            }
        }

        MeshNodeData* ResourceScene::getMeshNodeData(const QUuid &uuid) {
            return nodeMeshData[uuid];
        }

        const MeshNodeData* ResourceScene::getMeshNodeData(const QUuid &uuid) const {
            return nodeMeshData[uuid];
        }

        /**
         * @throws WS2Editor::Exception::IOException When failing to read the file
         */
        void ResourceScene::addModel(const QVector<WS2Common::Resource::ResourceMesh*> &meshes) {
            using namespace WS2Common::Scene;
            using namespace WS2Editor::Scene;

            //Get the static node
            WS2Common::Scene::SceneNode *staticNode = stage->getRootNode()->getChildByName(tr("Static"));
            //Create the static node if it doesn't exist
            if (!staticNode) {
                staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
                UI::ModelManager::modelOutliner->addNode(staticNode, stage->getRootNode());
            }

            for (int i = 0; i < meshes.size(); i++) {
                //The .split("@")[0] gets the part of the name before the @ symbol, which should be the name of the mesh
                //TODO: Make ResourceMesh store the name of a mesh, instead of doing string manip to get the name
                QString meshName = meshes.at(i)->getId().split("@")[0];
                MeshSceneNode *meshNode = new MeshSceneNode(meshName);
                meshNode->setMeshName(meshName);

                //Also make sure collision is generated on export
                MeshCollisionSceneNode *collision = new MeshCollisionSceneNode(tr("%1 Mesh Collision").arg(meshName));
                collision->setMeshName(meshName);
                meshNode->addChild(collision);

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

