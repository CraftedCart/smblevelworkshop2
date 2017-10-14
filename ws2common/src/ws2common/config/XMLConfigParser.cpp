#include "ws2common/config/XMLConfigParser.hpp"
#include "ws2common/MessageHandler.hpp"
#include <QXmlStreamReader>
#include <QDebug>
#include <QCoreApplication>

namespace WS2Common {
    namespace Config {
        namespace XMLConfigParser {
            Stage* parseStage(QString config) {
                Stage *stage = new Stage();

                //Read and parse the XML
                QXmlStreamReader xml(config);
                while (!xml.atEnd()) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "superMonkeyBallStage") {
                        //TODO: Check superMonkeyBallStage version attribute

                        //Parse the stages
                        while (!(xml.isEndElement() && xml.name() == "superMonkeyBallStage")) {
                            //Keep reading until the </superMonkeyBallStage> tag
                            xml.readNext();
                            if (!xml.isStartElement()) continue; //Ignore all end elements, again

                            if (xml.name() == "modelImport") {
                                qWarning() << "modelImport not yet implemented";
                                xml.skipCurrentElement();
                            } else if (xml.name() == "start") {
                                stage->getRootNode()->addChild(parseStart(xml));
                            } else if (xml.name() == "backgroundModel") {
                                stage->getFirstBackgroundGroup(true)->addChild(parseBackgroundModel(xml));
                            } else if (xml.name() == "falloutPlane") {
                                stage->setFalloutY(getAttribute(xml.attributes(), "y").toFloat());
                            } else if (xml.name() == "fog") {
                                qWarning() << "fog not yet implemented";
                                xml.skipCurrentElement();
                            } else if (xml.name() == "itemGroup") {
                                qWarning() << "itemGroup not yet implemented";
                                xml.skipCurrentElement();
                            } else {
                                qWarning().noquote() << "Unrecognised tag:" << xml.name();
                            }
                        }

                    } else {
                        qWarning().noquote() << QString("XML root tag is %1! It should be superMonkeyBallStage").arg(xml.name().toString());
                        //Don't spam the log with every single tag - Get outta here after the root one
                        //TODO: Throw an exception or something
                        break;
                    }
                }

                if (xml.hasError()) {
                    //TODO: Throw an exception or something
                    qWarning().noquote() << "XML parsing error:" << xml.errorString();
                }

                return stage;
            }

            Scene::StartSceneNode* parseStart(QXmlStreamReader &xml) {
                //Default name is "Start", translated
                Scene::StartSceneNode *start = new Scene::StartSceneNode(QCoreApplication::tr("XMLConfigParser", "Start"));

                while (!(xml.isEndElement() && xml.name() == "start")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        start->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        start->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        start->setRotation(getVec3Attributes(xml.attributes()));
                    }
                }

                return start;
            }

            Scene::MeshSceneNode* parseBackgroundModel(QXmlStreamReader &xml) {
                //A valid XML config should set the name of the bg model - It should never be UNDEFINED!
                Scene::MeshSceneNode *bg = new Scene::MeshSceneNode("UNDEFINED!");

                while (!(xml.isEndElement() && xml.name() == "backgroundModel")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        QString name = xml.readElementText();

                        bg->setName(name);
                        bg->setMeshName(name);
                    } else if (xml.name() == "position") {
                        bg->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        bg->setRotation(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "scale") {
                        bg->setScale(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "animKeyframes") { //TODO
                        qWarning() << "backgroundModel > animKeyframes not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "animLoopTime") { //TODO
                        qWarning() << "backgroundModel > animLoopTime not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "textureScroll") { //TODO
                        qWarning() << "backgroundModel > textureScroll not yet implemented!";
                        xml.skipCurrentElement();
                    }
                }

                return bg;
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
        }
    }
}

