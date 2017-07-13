#include "scene/MeshSceneNode.hpp"

namespace WS2 {
    namespace Scene {
        MeshSceneNode::MeshSceneNode(const QString name) : SceneNode(name) {}

        MeshSceneNode::MeshSceneNode(const QString name, Resource::ResourceMesh *mesh) : SceneNode(name) {
            this->mesh = mesh;
        }

        const Resource::ResourceMesh* MeshSceneNode::getMesh() const {
            return mesh;
        }
    }
}

