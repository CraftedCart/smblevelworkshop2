#include "ws2common/scene/MeshSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        MeshSceneNode::MeshSceneNode(const QString name) : SceneNode(name) {}

        void MeshSceneNode::setMeshName(QString meshName) {
            this->meshName = meshName;
        }

        const QString MeshSceneNode::getMeshName() const {
            return meshName;
        }

        void MeshSceneNode::setRuntimeReflective(bool runtimeReflective) {
            this->runtimeReflective = runtimeReflective;
        }

        bool MeshSceneNode::isRuntimeReflective() const {
            return runtimeReflective;
        }

        const QString MeshSceneNode::getSerializableName() const {
            return "meshSceneNode";
        }

        void MeshSceneNode::serializeNodeDataXml(QXmlStreamWriter &s) const {
            SceneNode::serializeNodeDataXml(s);

            s.writeStartElement("data-" + MeshSceneNode::getSerializableName());

            s.writeTextElement("meshName", meshName);
            s.writeTextElement("runtimeReflective", runtimeReflective ? "true" : "false");

            s.writeEndElement();
        }
    }
}

