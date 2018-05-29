#include "ws2common/scene/WormholeSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        WormholeSceneNode::WormholeSceneNode(const QString name) : SceneNode(name) {}

        const QUuid& WormholeSceneNode::getDestinationUuid() const {
            return destinationUuid;
        }

        void WormholeSceneNode::setDestinationUuid(QUuid destinationUuid) {
            this->destinationUuid = destinationUuid;
        }

        const QString WormholeSceneNode::getSerializableName() const {
            return "wormholeSceneNode";
        }

        void WormholeSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            SceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + WormholeSceneNode::getSerializableName());

            s.writeTextElement("destinationUuid", destinationUuid.toString());

            s.writeEndElement();
        }
    }
}

