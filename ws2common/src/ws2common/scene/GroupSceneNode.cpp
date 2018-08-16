#include "ws2common/scene/GroupSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        GroupSceneNode::GroupSceneNode(const QString name) : SceneNode(name) {}

        void GroupSceneNode::setCollisionGrid(CollisionGrid &collisionGrid) {
            this->collisionGrid = collisionGrid;
        }

        void GroupSceneNode::setCollisionGrid(CollisionGrid collisionGrid) {
            this->collisionGrid = collisionGrid;
        }

        CollisionGrid& GroupSceneNode::getCollisionGrid() {
            return collisionGrid;
        }

        const CollisionGrid& GroupSceneNode::getCollisionGrid() const {
            return collisionGrid;
        }

        const QString GroupSceneNode::getSerializableName() const {
            return "groupSceneNode";
        }

        void GroupSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            SceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + GroupSceneNode::getSerializableName());

            s.writeStartElement("collisionGrid");
            collisionGrid.serializeDataXml(s);
            s.writeEndElement();

            s.writeEndElement();
        }
    }
}

