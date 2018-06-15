#include "ws2common/scene/CollisionSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        CollisionSceneNode::CollisionSceneNode(const QString name) : SceneNode(name) {}

        const QString CollisionSceneNode::getSerializableName() const {
            return "collisionSceneNode";
        }

        void CollisionSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            SceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + CollisionSceneNode::getSerializableName());
            //Well this is useless for the moment... But maybe something will fill this space... maybe?
            s.writeEndElement();
        }
    }
}

