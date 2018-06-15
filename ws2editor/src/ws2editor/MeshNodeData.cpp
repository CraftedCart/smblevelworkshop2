#include "ws2editor/MeshNodeData.hpp"

namespace WS2Editor {
    using namespace WS2Editor::Physics;
    using namespace WS2Common;
    using namespace WS2Common::Scene;
    using namespace WS2Common::Resource;
    using namespace WS2Common::Model;

    MeshNodeData::MeshNodeData(SceneNode *node, ResourceMesh *mesh) :
        node(node),
        meshes(QVector<ResourceMesh*> {mesh}) {
        physicsContainer = new PhysicsContainer(node, meshes, node->getTransform());
    }

    MeshNodeData::MeshNodeData(SceneNode *node, QVector<ResourceMesh*>& meshes) :
        node(node),
        meshes(meshes) {
        physicsContainer = new PhysicsContainer(node, meshes, node->getTransform());
    }

    const SceneNode* MeshNodeData::getNode() const {
        return node;
    }

    QVector<ResourceMesh*>& MeshNodeData::getMeshes() {
        return meshes;
    }

    const QVector<ResourceMesh*>& MeshNodeData::getMeshes() const {
        return meshes;
    }

    QVector<MeshSegment*> MeshNodeData::getMeshSegments() {
        QVector<WS2Common::Model::MeshSegment*> segments;
        for (ResourceMesh *mesh : meshes) segments.append(mesh->getMeshSegments());
        return segments;
    }

    const QVector<MeshSegment*> MeshNodeData::getMeshSegments() const {
        QVector<WS2Common::Model::MeshSegment*> segments;
        for (ResourceMesh *mesh : meshes) segments.append(mesh->getMeshSegments());
        return segments;
    }

    PhysicsContainer* MeshNodeData::getPhysicsContainer() {
        return physicsContainer;
    }

    const PhysicsContainer* MeshNodeData::getPhysicsContainer() const {
        return physicsContainer;
    }

    const AABB3 MeshNodeData::getAabb() const {
        if (meshes.isEmpty()) return AABB3();
        if (meshes.size() == 1) return meshes.at(0)->getAabb();

        AABB3 aabb = meshes.at(0)->getAabb();

        for (int i = 1; i < meshes.size(); i++) {
            aabb.mergeWith(meshes.at(i)->getAabb());
        }

        return aabb;
    }
}

