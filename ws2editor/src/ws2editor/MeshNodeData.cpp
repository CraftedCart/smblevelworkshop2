#include "ws2editor/MeshNodeData.hpp"

namespace WS2Editor {
    MeshNodeData::MeshNodeData(SceneNode *node, ResourceMesh *mesh) : node(node), mesh(mesh) {
        physicsContainer = new PhysicsContainer(node, mesh, node->getTransform());
    }

    const SceneNode* MeshNodeData::getNode() const {
        return node;
    }

    ResourceMesh* MeshNodeData::getMesh() {
        return mesh;
    }

    const ResourceMesh* MeshNodeData::getMesh() const {
        return mesh;
    }

    PhysicsContainer* MeshNodeData::getPhysicsContainer() {
        return physicsContainer;
    }

    const PhysicsContainer* MeshNodeData::getPhysicsContainer() const {
        return physicsContainer;
    }
}

