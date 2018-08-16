#include "ws2common/scene/BananaSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        BananaSceneNode::BananaSceneNode(const QString name) : SceneNode(name) {}

        EnumBananaType BananaSceneNode::getType() const {
            return type;
        }

        void BananaSceneNode::setType(EnumBananaType type) {
            this->type = type;
        }

        const QString BananaSceneNode::getSerializableName() const {
            return "bananaSceneNode";
        }

        void BananaSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            SceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + BananaSceneNode::getSerializableName());

            s.writeTextElement("type", BananaType::toString(type));

            s.writeEndElement();
        }
    }
}

