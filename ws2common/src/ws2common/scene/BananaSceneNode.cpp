#include "ws2common/scene/BananaSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        BananaSceneNode::BananaSceneNode(const QString name) : SceneNode(name) {}

        EnumBananaType BananaSceneNode::getType() {
            return type;
        }

        void BananaSceneNode::setType(EnumBananaType type) {
            this->type = type;
        }
    }
}

