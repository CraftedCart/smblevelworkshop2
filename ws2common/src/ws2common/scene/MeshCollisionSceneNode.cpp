#include "ws2common/scene/MeshCollisionSceneNode.hpp"
#include <QDebug>

namespace WS2Common {
    namespace Scene {
        MeshCollisionSceneNode::MeshCollisionSceneNode(const QString name) : CollisionSceneNode(name) {}

        void MeshCollisionSceneNode::setMeshName(QString meshName) {
            this->meshName = meshName;
        }

        const QString MeshCollisionSceneNode::getMeshName() const {
            return meshName;
        }

        const QString MeshCollisionSceneNode::getSerializableName() const {
            return "meshCollisionSceneNode";
        }

        void MeshCollisionSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            CollisionSceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + MeshCollisionSceneNode::getSerializableName());

            s.writeTextElement("meshName", meshName);

            s.writeEndElement();
        }
    }
}

