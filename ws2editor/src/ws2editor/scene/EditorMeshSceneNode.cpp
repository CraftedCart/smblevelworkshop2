#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/resource/ResourceScene.hpp"

namespace WS2Editor {
    namespace Scene {
        using namespace WS2Editor::Resource;
        using namespace WS2Common::Resource;

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, ResourceScene *scene) :
            MeshSceneNode(name),
            scene(scene) {
            initPhysics();
        }

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, ResourceScene *scene, WS2Common::Resource::ResourceMesh *mesh) :
            MeshSceneNode(name),
            scene(scene) {
            this->mesh = mesh;
            initPhysics();
        }

        EditorMeshSceneNode::~EditorMeshSceneNode() {
            //mesh is not deleted as ResourceManager owns ResourceMeshs
            scene->getPhysicsManager()->removeRigidBody(physicsContainer->getRigidBody());

            delete physicsContainer;
        }

        void EditorMeshSceneNode::initPhysics() {
            physicsContainer = new PhysicsContainer(this, mesh, transform);

            //Register the physics object
            scene->getPhysicsManager()->addRigidBody(physicsContainer->getRigidBody());
        }

        const WS2Common::Resource::ResourceMesh* EditorMeshSceneNode::getMesh() const {
            return mesh;
        }

        btRigidBody* EditorMeshSceneNode::getPhysicsRigidBody() {
            return physicsContainer->getRigidBody();
        }
    }
}

