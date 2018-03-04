#include "ws2common/scene/SwitchSceneNode.hpp"
#include <QDebug>

namespace WS2Common {
    namespace Scene {
        SwitchSceneNode::SwitchSceneNode(const QString name) : SceneNode(name) {}

        EnumPlaybackState SwitchSceneNode::getType() const {
            return type;
        }

        void SwitchSceneNode::setType(EnumPlaybackState type) {
            this->type = type;
        }

        unsigned short SwitchSceneNode::getLinkedAnimGroupId() const {
            return linkedAnimGroupId;
        }

        void SwitchSceneNode::setLinkedAnimGroupId(unsigned short linkedAnimGroupId) {
            this->linkedAnimGroupId = linkedAnimGroupId;
        }
    }
}

