#include "ws2common/scene/StartSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        StartSceneNode::StartSceneNode(const QString name) : SceneNode(name) {}

        const QString StartSceneNode::getSerializableName() const {
            return "startSceneNode";
        }
    }
}

