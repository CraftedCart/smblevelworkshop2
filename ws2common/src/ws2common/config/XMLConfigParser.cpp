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
                                qWarning() << "modelImport not yet implemented!";
                                xml.skipCurrentElement();
                            } else if (xml.name() == "start") {
                                stage->getRootNode()->addChild(parseStart(xml));
                            } else if (xml.name() == "backgroundModel") {
                                stage->getFirstBackgroundGroup(true)->addChild(parseBackgroundModel(xml));
                            } else if (xml.name() == "falloutPlane") {
                                stage->setFalloutY(getAttribute(xml.attributes(), "y").toFloat());
                            } else if (xml.name() == "fog") {
                                qWarning() << "fog not yet implemented!";
                                xml.skipCurrentElement();
                            } else if (xml.name() == "itemGroup") {
                                stage->getRootNode()->addChild(parseItemGroup(xml));
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

                qDebug() << stage->getRootNode();

                if (xml.hasError()) {
                    //TODO: Throw an exception or something
                    qWarning().noquote() << "XML parsing error:" << xml.errorString();
                }

                return stage;
            }

            Scene::StartSceneNode* parseStart(QXmlStreamReader &xml) {
                //Default name is "Start", translated
                Scene::StartSceneNode *start = new Scene::StartSceneNode(QCoreApplication::translate("XMLConfigParser", "Start"));

                while (!(xml.isEndElement() && xml.name() == "start")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        start->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        start->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        start->setRotation(getVec3Attributes(xml.attributes()));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: start >" << xml.name();
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
                    } else {
                        qWarning().noquote() << "Unrecognised tag: backgroundModel >" << xml.name();
                    }
                }

                return bg;
            }

            Scene::GroupSceneNode* parseItemGroup(QXmlStreamReader &xml) {
                //Default name is "Group", translated
                Scene::GroupSceneNode *group = new Scene::GroupSceneNode(QCoreApplication::tr("XMLConfigParser", "Group"));

                while (!(xml.isEndElement() && xml.name() == "itemGroup")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        group->setName(xml.readElementText());
                    } else if (xml.name() == "rotationCenter") { //TODO
                        qWarning() << "itemGroup > rotationCenter not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "initialRotation") { //TODO
                        qWarning() << "itemGroup > initialRotation not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "animSeesawType") { //TODO
                        qWarning() << "itemGroup > animSeesawType not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "seesawSensitivity") { //TODO
                        qWarning() << "itemGroup > seesawSensitivity not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "seesawResetStiffness") { //TODO
                        qWarning() << "itemGroup > seesawResetStiffness not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "seesawRotationBounds") { //TODO
                        qWarning() << "itemGroup > seesawRotationBounds not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "animKeyframes") { //TODO
                        qWarning() << "itemGroup > animKeyframes not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "animLoopTime") { //TODO
                        qWarning() << "itemGroup > animLoopTime not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "animGroupID") { //TODO
                        qWarning() << "itemGroup > animGroupID not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "animInitialState") { //TODO
                        qWarning() << "itemGroup > animInitialState not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "collisionGrid") { //TODO
                        qWarning() << "itemGroup > collisionGrid not yet implemented!";
                        xml.skipCurrentElement();
                    } else if (xml.name() == "goal") {
                        group->addChild(parseGoal(xml));
                    } else if (xml.name() == "bumper") {
                        group->addChild(parseBumper(xml));
                    } else if (xml.name() == "jamabar") {
                        group->addChild(parseJamabar(xml));
                    } else if (xml.name() == "banana") {
                        group->addChild(parseBanana(xml));
                    } else if (xml.name() == "falloutVolume") {
                        group->addChild(parseFalloutVolume(xml));
                    } else if (xml.name() == "wormhole") { //TODO
                        qWarning() << "itemGroup > wormhole not yet implemented!";
                        xml.skipCurrentElement();
                        //TODO: Store wormhole destinations in a map
                        //TODO: Then link wormholes after processing
                    } else if (xml.name() == "switch") {
                        group->addChild(parseSwitch(xml));
                    } else if (xml.name() == "levelModel") { //TODO
                        group->addChild(parseLevelModel(xml));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: itemGroup >" << xml.name();
                    }
                }

                return group;
            }

            Scene::GoalSceneNode* parseGoal(QXmlStreamReader &xml) {
                //Default name is "Goal x", translated
                unsigned int id = 0;
                Scene::GoalSceneNode *goal = new Scene::GoalSceneNode(QCoreApplication::translate("XMLConfigParser", "Goal %1").arg(id));

                while (!(xml.isEndElement() && xml.name() == "goal")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        goal->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        goal->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        goal->setRotation(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "type") {
                        goal->setType(GoalType::fromString(xml.readElementText()));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: goal >" << xml.name();
                    }
                }

                return goal;
            }

            Scene::BumperSceneNode* parseBumper(QXmlStreamReader &xml) {
                //Default name is "Bumper x", translated
                unsigned int id = 0;
                Scene::BumperSceneNode *bumper = new Scene::BumperSceneNode(QCoreApplication::translate("XMLConfigParser", "Bumper %1").arg(id));

                while (!(xml.isEndElement() && xml.name() == "bumper")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        bumper->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        bumper->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        bumper->setRotation(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "scale") {
                        bumper->setScale(getVec3Attributes(xml.attributes()));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: bumper >" << xml.name();
                    }
                }

                return bumper;
            }

            Scene::JamabarSceneNode* parseJamabar(QXmlStreamReader &xml) {
                //Default name is "Jamabar x", translated
                unsigned int id = 0;
                Scene::JamabarSceneNode *jamabar = new Scene::JamabarSceneNode(QCoreApplication::translate("XMLConfigParser", "Jamabar %1").arg(id));

                while (!(xml.isEndElement() && xml.name() == "jamabar")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        jamabar->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        jamabar->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        jamabar->setRotation(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "scale") {
                        jamabar->setScale(getVec3Attributes(xml.attributes()));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: jamabar >" << xml.name();
                    }
                }

                return jamabar;
            }

            Scene::BananaSceneNode* parseBanana(QXmlStreamReader &xml) {
                //Default name is "Banana x", translated
                unsigned int id = 0;
                Scene::BananaSceneNode *banana = new Scene::BananaSceneNode(QCoreApplication::translate("XMLConfigParser", "Banana %1").arg(id));

                while (!(xml.isEndElement() && xml.name() == "banana")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        banana->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        banana->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "type") {
                        banana->setType(BananaType::fromString(xml.readElementText()));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: banana >" << xml.name();
                    }
                }

                return banana;
            }

            Scene::FalloutVolumeSceneNode* parseFalloutVolume(QXmlStreamReader &xml) {
                //Default name is "Fallout Volume x", translated
                unsigned int id = 0;
                Scene::FalloutVolumeSceneNode *volume = new Scene::FalloutVolumeSceneNode(QCoreApplication::translate("XMLConfigParser", "Fallout Volume %1").arg(id));

                while (!(xml.isEndElement() && xml.name() == "falloutVolume")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        volume->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        volume->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        volume->setRotation(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "scale") {
                        volume->setScale(getVec3Attributes(xml.attributes()));
                    } else {
                        qWarning().noquote() << "Unrecognised tag: falloutVolume >" << xml.name();
                    }
                }

                return volume;
            }

            Scene::SwitchSceneNode* parseSwitch(QXmlStreamReader &xml) {
                //Default name is "Switch x", translated
                unsigned int id = 0;
                //Woo can't use switch as a variable name!
                Scene::SwitchSceneNode *sw = new Scene::SwitchSceneNode(QCoreApplication::translate("XMLConfigParser", "Switch %1").arg(id));

                while (!(xml.isEndElement() && xml.name() == "switch")) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "name") {
                        sw->setName(xml.readElementText());
                    } else if (xml.name() == "position") {
                        sw->setPosition(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "rotation") {
                        sw->setRotation(getVec3Attributes(xml.attributes()));
                    } else if (xml.name() == "type") {
                        sw->setType(PlaybackState::fromString(xml.readElementText()));
                    } else if (xml.name() == "animGroupID") {
                        sw->setAnimGroupId(xml.readElementText().toUShort());
                    } else {
                        qWarning().noquote() << "Unrecognised tag: switch >" << xml.name();
                    }
                }

                return sw;
            }

            Scene::MeshSceneNode* parseLevelModel(QXmlStreamReader &xml) {
                QString name = xml.readElementText();
                Scene::MeshSceneNode *mesh = new Scene::MeshSceneNode(name);
                mesh->setMeshName(name);
                return mesh;
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

