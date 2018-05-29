#include "ws2common/scene/WormholeSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        WormholeSceneNode::WormholeSceneNode(const QString name) : SceneNode(name) {}

        const QUuid& WormholeSceneNode::getDestinationUuid() const {
            return destinationUuid;
        }

        void WormholeSceneNode::setDestinationUuid(QUuid destinationUuid) {
            this->destinationUuid = destinationUuid;
        }
    }
}

