#include "scene/MeshSceneNode.hpp"

namespace WS2 {
    namespace Scene {
        MeshSceneNode::MeshSceneNode() {}

        MeshSceneNode::MeshSceneNode(Resource::ResourceMesh *mesh) {
            this->mesh = mesh;
        }

        const Resource::ResourceMesh* MeshSceneNode::getMesh() const {
            return mesh;
        }
    }
}

