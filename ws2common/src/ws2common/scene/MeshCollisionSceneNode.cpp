#include "ws2common/scene/MeshCollisionSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        MeshCollisionSceneNode::MeshCollisionSceneNode(const QString name) : CollisionSceneNode(name) {}

        void MeshCollisionSceneNode::setMeshName(QString meshName) {
            this->meshName = meshName;
        }

        const QString MeshCollisionSceneNode::getMeshName() const {
            return meshName;
        }
    }
}

