#include "ws2common/scene/WormholeSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        WormholeSceneNode::WormholeSceneNode(const QString name) : SceneNode(name) {}

        WormholeSceneNode* WormholeSceneNode::getDestination() {
            return destination;
        }

        void WormholeSceneNode::setDestination(WormholeSceneNode *destination) {
            this->destination = destination;
        }
    }
}

