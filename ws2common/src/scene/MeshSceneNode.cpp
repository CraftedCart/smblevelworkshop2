#include "scene/MeshSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        MeshSceneNode::MeshSceneNode(QString name) : SceneNode(name) {}

        void MeshSceneNode::setMeshName(QString meshName) {
            this->meshName = meshName;
        }

        QString MeshSceneNode::getMeshName() {
            return meshName;
        }
    }
}

