#include "ws2common/scene/GolfHoleSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        GolfHoleSceneNode::GolfHoleSceneNode(const QString name) : SceneNode(name) {}

        const QString GolfHoleSceneNode::getSerializableName() const {
            return "golfHoleSceneNode";
        }
    }
}

