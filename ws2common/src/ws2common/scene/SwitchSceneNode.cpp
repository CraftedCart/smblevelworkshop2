#include "ws2common/scene/SwitchSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        SwitchSceneNode::SwitchSceneNode(const QString name) : SceneNode(name) {}

        EnumPlaybackState SwitchSceneNode::getType() {
            return type;
        }

        void SwitchSceneNode::setType(EnumPlaybackState type) {
            this->type = type;
        }
    }
}

