#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/resource/ResourceScene.hpp"

namespace WS2Editor {
    namespace Scene {
        using namespace WS2Editor::Resource;
        using namespace WS2Common::Resource;

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, ResourceScene *scene) :
            MeshSceneNode(name),
            scene(scene) {
            //initPhysics();
        }

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, ResourceScene *scene, WS2Common::Resource::ResourceMesh *mesh) :
            MeshSceneNode(name),
            scene(scene) {
            initPhysics(mesh);
        }

        EditorMeshSceneNode::~EditorMeshSceneNode() {

        }

        void EditorMeshSceneNode::initPhysics(ResourceMesh *mesh) {
            scene->addMeshNodeData(this, new MeshNodeData(this, mesh));
        }

        const WS2Common::Resource::ResourceMesh* EditorMeshSceneNode::getMesh() const {
            return scene->getMeshNodeData(this)->getMesh();
        }

        btRigidBody* EditorMeshSceneNode::getPhysicsRigidBody() {
            return scene->getMeshNodeData(this)->getPhysicsContainer()->getRigidBody();
        }
    }
}

