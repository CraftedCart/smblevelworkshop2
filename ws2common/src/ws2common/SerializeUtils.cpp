#include "ws2common/SerializeUtils.hpp"
#include <QDebug>

namespace WS2Common {
    namespace SerializeUtils {
        using namespace WS2Common::Scene;

        SceneNode* deserializeNodeFromXml(QXmlStreamReader &xml, SceneNode *parent) {
            if (xml.name() == "node-sceneNode") {
                SceneNode *node = new SceneNode();
                parseSceneNode(xml, node);
                if (parent != nullptr) parent->addChild(node);
                return node;
            } else if (xml.name() == "node-meshSceneNode") {
                MeshSceneNode *node = new MeshSceneNode();
                parseMeshSceneNode(xml, node);
                if (parent != nullptr) parent->addChild(node);
                return node;
            }

            return nullptr; //Failed to deserialize
        }

        void parseSceneNode(QXmlStreamReader &xml, SceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "node-sceneNode")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "data") {
                    while (!(xml.isEndElement() && xml.name() == "data")) {
                        xml.readNext();
                        if (!xml.isStartElement()) continue; //Ignore all end elements

                        if (xml.name() == "data-sceneNode") {
                            parseSceneNodeData(xml, node);
                        } else {
                            qCritical().noquote() << "Invalid element data > " + xml.name();
                        }
                    }

                } else if (xml.name() == "children") {
                    parseChildren(xml, node);
                } else {
                    qCritical().noquote() << "Invalid element node-sceneNode > " + xml.name();
                }
            }
        }

        void parseSceneNodeData(QXmlStreamReader &xml, SceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "data-sceneNode")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "name") {
                    node->setName(xml.readElementText());
                } else if (xml.name() == "uuid") {
                    node->setUuid(QUuid(xml.readElementText()));
                } else {
                    qCritical().noquote() << "Invalid element data-sceneNode > " + xml.name();
                }
            }
        }

        void parseMeshSceneNode(QXmlStreamReader &xml, MeshSceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "node-meshSceneNode")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "data") {
                    while (!(xml.isEndElement() && xml.name() == "data")) {
                        xml.readNext();
                        if (!xml.isStartElement()) continue; //Ignore all end elements

                        if (xml.name() == "data-sceneNode") {
                            parseSceneNodeData(xml, node);
                        } else if (xml.name() == "data-meshSceneNode") {
                            parseMeshSceneNodeData(xml, node);
                        } else {
                            qCritical().noquote() << "Invalid element data > " + xml.name();
                        }
                    }

                } else if (xml.name() == "children") {
                    parseChildren(xml, node);
                } else {
                    qCritical().noquote() << "Invalid element node-meshSceneNode > " + xml.name();
                }
            }
        }

        void parseMeshSceneNodeData(QXmlStreamReader &xml, MeshSceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "data-meshSceneNode")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "meshName") {
                    node->setMeshName(xml.readElementText());
                } else {
                    qCritical().noquote() << "Invalid element data-meshSceneNode > " + xml.name();
                }
            }
        }

        void parseChildren(QXmlStreamReader &xml, SceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "children")) {
                xml.readNext();
                deserializeNodeFromXml(xml, node);
            }
        }

        void writeVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::vec2 vec,
                QString xAttrName,
                QString yAttrName
                ) {
            xml.writeStartElement(name);
            xml.writeAttribute(xAttrName, QString::number(vec.x));
            xml.writeAttribute(yAttrName, QString::number(vec.y));
            xml.writeEndElement();
        }

        void writeVec3(
                QXmlStreamWriter &xml,
                QString name,
                glm::vec3 vec,
                QString xAttrName,
                QString yAttrName,
                QString zAttrName
                ) {
            xml.writeStartElement(name);
            xml.writeAttribute(xAttrName, QString::number(vec.x));
            xml.writeAttribute(yAttrName, QString::number(vec.y));
            xml.writeAttribute(zAttrName, QString::number(vec.z));
            xml.writeEndElement();
        }
    }
}

