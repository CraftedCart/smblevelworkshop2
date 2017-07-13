#include "scene/MeshSceneNode.hpp"

namespace WS2 {
    namespace Scene {
        MeshSceneNode::MeshSceneNode() {}

        MeshSceneNode::MeshSceneNode(Resource::ResourceMesh *mesh) {
            this->mesh = mesh;
        }

        Resource::ResourceMesh* MeshSceneNode::getMesh() {
            return mesh;
        }
    }
}

