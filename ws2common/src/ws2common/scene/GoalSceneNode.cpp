#include "ws2common/scene/GoalSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        GoalSceneNode::GoalSceneNode(const QString name) : SceneNode(name) {}

        EnumGoalType GoalSceneNode::getType() const {
            return type;
        }

        void GoalSceneNode::setType(EnumGoalType type) {
            this->type = type;
        }

        const QString GoalSceneNode::getSerializableName() const {
            return "goalSceneNode";
        }

        bool GoalSceneNode::getCastShadow() const {
            return castShadow;
        }

        void GoalSceneNode::setCastShadow(bool castShadow) {
            this->castShadow = castShadow;
        }

        void GoalSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            SceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + GoalSceneNode::getSerializableName());

            s.writeTextElement("type", GoalType::toString(type));

            s.writeEndElement();
        }
    }
}

