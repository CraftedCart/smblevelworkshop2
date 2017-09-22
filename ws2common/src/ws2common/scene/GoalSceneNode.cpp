#include "ws2common/scene/GoalSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        GoalSceneNode::GoalSceneNode(const QString name) : SceneNode(name) {}

        EnumGoalType GoalSceneNode::getType() {
            return type;
        }

        void GoalSceneNode::setType(EnumGoalType type) {
            this->type = type;
        }
    }
}

