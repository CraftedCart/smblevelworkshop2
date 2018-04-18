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
            } else if (xml.name() == "node-groupSceneNode") {
                GroupSceneNode *node = new GroupSceneNode();
                parseGroupSceneNode(xml, node);
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
                            qWarning().noquote() << "Unrecognised tag: data > " + xml.name();
                        }
                    }

                } else if (xml.name() == "children") {
                    parseChildren(xml, node);
                } else {
                    qWarning().noquote() << "Unrecognised tag: node-sceneNode > " + xml.name();
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
                    qWarning().noquote() << "Unrecognised tag: data-sceneNode > " + xml.name();
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
                            qWarning().noquote() << "Unrecognised tag: data > " + xml.name();
                        }
                    }

                } else if (xml.name() == "children") {
                    parseChildren(xml, node);
                } else {
                    qWarning().noquote() << "Unrecognised tag: node-meshSceneNode > " + xml.name();
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
                    qWarning().noquote() << "Unrecognised tag: data-meshSceneNode > " + xml.name();
                }
            }
        }

        void parseGroupSceneNode(QXmlStreamReader &xml, GroupSceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "node-groupSceneNode")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "data") {
                    while (!(xml.isEndElement() && xml.name() == "data")) {
                        xml.readNext();
                        if (!xml.isStartElement()) continue; //Ignore all end elements

                        if (xml.name() == "data-sceneNode") {
                            parseSceneNodeData(xml, node);
                        } else if (xml.name() == "data-groupSceneNode") {
                            parseGroupSceneNodeData(xml, node);
                        } else {
                            qWarning().noquote() << "Unrecognised tag: data > " + xml.name();
                        }
                    }

                } else if (xml.name() == "children") {
                    parseChildren(xml, node);
                } else {
                    qWarning().noquote() << "Unrecognised tag: node-groupSceneNode > " + xml.name();
                }
            }
        }

        void parseGroupSceneNodeData(QXmlStreamReader &xml, GroupSceneNode *node) {
            while (!(xml.isEndElement() && xml.name() == "data-groupSceneNode")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "collisionGrid") {
                    node->setCollisionGrid(CollisionGrid::deserializeDataXml(xml));
                } else {
                    qWarning().noquote() << "Unrecognised tag: data-groupSceneNode > " + xml.name();
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

        void writeUVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::uvec2 vec,
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

        QStringRef getAttribute(const QXmlStreamAttributes &attrs, const QString attrName) {
            foreach(const QXmlStreamAttribute &attr, attrs) {
                if (attr.name() == attrName) return attr.value();
            }

            //Oh noes - not found
            return QStringRef();
        }

        glm::vec3 getVec3Attributes(const QXmlStreamAttributes &attrs,
                const QString x, const QString y, const QString z) {
            glm::vec3 vec;
            foreach(const QXmlStreamAttribute &attr, attrs) {
                if (attr.name() == x) vec.x = attr.value().toFloat();
                else if (attr.name() == y) vec.y = attr.value().toFloat();
                else if (attr.name() == z) vec.z = attr.value().toFloat();
            }

            return vec;
        }

        glm::vec2 getVec2Attributes(const QXmlStreamAttributes &attrs,
                const QString x, const QString y) {
            glm::vec2 vec;
            foreach(const QXmlStreamAttribute &attr, attrs) {
                if (attr.name() == x) vec.x = attr.value().toFloat();
                else if (attr.name() == y) vec.y = attr.value().toFloat();
            }

            return vec;
        }

        glm::uvec2 getUVec2Attributes(const QXmlStreamAttributes &attrs,
                const QString x, const QString y) {
            glm::uvec2 vec;
            foreach(const QXmlStreamAttribute &attr, attrs) {
                if (attr.name() == x) vec.x = attr.value().toFloat();
                else if (attr.name() == y) vec.y = attr.value().toFloat();
            }

            return vec;
        }
    }
}

