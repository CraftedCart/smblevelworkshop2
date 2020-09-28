#include "ws2common/scene/BoosterSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        BoosterSceneNode::BoosterSceneNode(const QString name) : SceneNode(name) {}

        const QString BoosterSceneNode::getSerializableName() const {
            return "boosterSceneNode";
        }
    }
}

