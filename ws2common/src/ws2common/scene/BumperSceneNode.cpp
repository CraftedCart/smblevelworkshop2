#include "ws2common/scene/BumperSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        BumperSceneNode::BumperSceneNode(const QString name) : SceneNode(name) {}

        const QString BumperSceneNode::getSerializableName() const {
            return "bumperSceneNode";
        }
    }
}

