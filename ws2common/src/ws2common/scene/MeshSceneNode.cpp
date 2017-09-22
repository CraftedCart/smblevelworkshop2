#include "ws2common/scene/MeshSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        MeshSceneNode::MeshSceneNode(const QString name) : SceneNode(name) {}

        void MeshSceneNode::setMeshName(QString meshName) {
            this->meshName = meshName;
        }

        QString MeshSceneNode::getMeshName() {
            return meshName;
        }
    }
}

