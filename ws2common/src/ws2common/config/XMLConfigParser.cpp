#include "ws2common/config/XMLConfigParser.hpp"
#include "ws2common/scene/MeshCollisionSceneNode.hpp"
#include "ws2common/SerializeUtils.hpp"
#include "ws2common/MathUtils.hpp"
#include <QXmlStreamReader>
#include <QDebug>
#include <QCoreApplication>
#include <QtMath>

namespace WS2Common {
    namespace Config {
        Stage* XMLConfigParser::parseStage(QString config, QDir relativeRoot) {
            Stage *stage = new Stage();

            //Read and parse the XML
            QXmlStreamReader xml(config);
            while (!xml.atEnd()) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (xml.name() == "superMonkeyBallStage") { //TODO: Check superMonkeyBallStage version attribute
                    //Parse the stages
                    while (!(xml.isEndElement() && xml.name() == "superMonkeyBallStage")) {
                        //Keep reading until the </superMonkeyBallStage> tag
                        xml.readNext();
                        if (!xml.isStartElement()) continue; //Ignore all end elements, again

                        qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                            "superMonkeyBallStage > " << xml.name();

                        if (xml.name() == "modelImport") {
                            QUrl url = parseModelImport(xml, relativeRoot);
                            if (!url.isEmpty()) stage->addModel(url);
                        } else if (xml.name() == "start") {
                            stage->getRootNode()->addChild(parseStart(xml));
                        } else if (xml.name() == "backgroundModel") {
                            stage->getFirstBackgroundGroup(true)->addChild(parseBackgroundModel(xml));
                        } else if (xml.name() == "foregroundModel") {
                            stage->getFirstForegroundGroup(true)->addChild(parseForegroundModel(xml));
                        } else if (xml.name() == "falloutPlane") {
                            stage->setFalloutY(SerializeUtils::getAttribute(xml.attributes(), "y").toFloat());
                        } else if (xml.name() == "stageType") {
                            stage->setStageType(StageType::fromString(xml.readElementText()));
                        } else if (xml.name() == "fog") {
                            stage->setFog(parseFog(xml));
                        } else if (xml.name() == "fogAnimationKeyframes") {
                            stage->setFogAnimation(parseFogAnimation(xml));
                        } else if (xml.name() == "itemGroup") {
                            stage->getRootNode()->addChild(parseItemGroup(xml));
                        } else if (xml.name() == "trackPath") {
                            stage->getRootNode()->addChild(parseTrackPath(xml));
                        } else if (xml.name() == "booster") {
                            stage->getRootNode()->addChild(parseBooster(xml));
                        } else if (xml.name() == "golfHole") {
                            stage->getRootNode()->addChild(parseGolfHole(xml));
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

            //Done parsing - now link wormholes
            if (wormholeDestMap.size() > 0) qInfo().noquote() << QString("Linking %1 wormholes...").arg(wormholeDestMap.size());

            QHashIterator<Scene::WormholeSceneNode*, QString> i(wormholeDestMap);
            while (i.hasNext()) {
                i.next();

                //Iterate over all wormholes to find the one with the matching destination name
                QHashIterator<Scene::WormholeSceneNode*, QString> j(wormholeDestMap);
                bool linkSuccess = false;
                while (j.hasNext()) {
                    j.next();

                    if (i.value() == j.key()->getName()) {
                        i.key()->setDestinationUuid(j.key()->getUuid());
                        linkSuccess = true;
                        break;
                    }
                }

                //If we couldn't find a wormhole to link against, warn the user
                if (!linkSuccess) {
                    qCritical().noquote() << QString("Failed to link wormhole \"%1\" to \"%2\" - \"%2\" doesn't exist").arg(j.key()->getName()).arg(i.value());
                }
            }

            return stage;
        }

        QUrl XMLConfigParser::parseModelImport(QXmlStreamReader &xml, QDir relativeRoot) {
            QString value = xml.readElementText();

            if (value.startsWith("//")) { //Beginning with // denotes a relative path
                QFileInfo fileInfo(relativeRoot, value.mid(2));

                //Check if the file is missing, and return am empty QUrl if so
                if (!fileInfo.exists()) {
                    qWarning().noquote() << "modelImport file missing:" << fileInfo.absoluteFilePath();
                    return QUrl();
                }

                return QUrl::fromLocalFile(fileInfo.absoluteFilePath());
            } else { //Else it should be a URL (Like file:///something/or/other.obj)
                QUrl url(value);
                QFileInfo fileInfo(url.toLocalFile());

                //Check if the file is missing, and return am empty QUrl if so
                if (!fileInfo.exists()) {
                    qWarning().noquote() << "modelImport file missing:" << fileInfo.absoluteFilePath();
                    return QUrl();
                }

                return url;
            }
        }

        Scene::StartSceneNode* XMLConfigParser::parseStart(QXmlStreamReader &xml) {
            //Default name is "Start", translated
            Scene::StartSceneNode *start = new Scene::StartSceneNode(QCoreApplication::translate("XMLConfigParser", "Start"));

            while (!(xml.isEndElement() && xml.name() == "start")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "start > " << xml.name();

                if (xml.name() == "name") {
                    start->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    start->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    start->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "playerID") {
                    start->setPlayerID(xml.readElementText().toUInt());
                } else {
                    qWarning().noquote() << "Unrecognised tag: start >" << xml.name();
                }
            }

            return start;
        }

        Scene::MeshSceneNode* XMLConfigParser::parseBackgroundModel(QXmlStreamReader &xml) {
            //A valid XML config should set the name of the bg model - It should never be UNDEFINED!
            Scene::MeshSceneNode *bg = new Scene::MeshSceneNode("UNDEFINED!");
            Animation::TransformAnimation *anim = nullptr;
            float loopTime = 0.0f;

            while (!(xml.isEndElement() && xml.name() == "backgroundModel")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "backgroundModel > " << xml.name();

                if (xml.name() == "name") {
                    QString name = xml.readElementText();

                    bg->setName(name);
                    bg->setMeshName(name);
                } else if (xml.name() == "position") {
                    bg->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    bg->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "scale") {
                    bg->setScale(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "meshType") {
                    bg->setMeshType(xml.readElementText().toUInt());
                } else if (xml.name() == "animKeyframes") { 
                    Animation::TransformAnimation *transformAnim = parseTransformAnimation(xml, true);
                    bg->setTransformAnimation(transformAnim);
                    anim = transformAnim; //For later linking (So that the loop type is set in the transformAnim)
                } else if (xml.name() == "animLoopTime") { 
                    loopTime = (xml.readElementText().toFloat());
                } else if (xml.name() == "textureScroll") {
                   bg->setTextureScroll(SerializeUtils::getVec2Attributes(xml.attributes()));
                } else {
                    qWarning().noquote() << "Unrecognised tag: backgroundModel >" << xml.name();
                }

            }

            if (anim != nullptr) {
                anim->setLoopTime(loopTime);
            }
            return bg;
        }

        Scene::MeshSceneNode* XMLConfigParser::parseForegroundModel(QXmlStreamReader &xml) {
            //A valid XML config should set the name of the fg model - It should never be UNDEFINED!
            Scene::MeshSceneNode *fg = new Scene::MeshSceneNode("UNDEFINED!");
            Animation::TransformAnimation *anim = nullptr;
            float loopTime = 0.0f;

            while (!(xml.isEndElement() && xml.name() == "foregroundModel")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "foregroundModel > " << xml.name();

                if (xml.name() == "name") {
                    QString name = xml.readElementText();

                    fg->setName(name);
                    fg->setMeshName(name);
                } else if (xml.name() == "position") {
                    fg->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    fg->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "scale") {
                    fg->setScale(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "meshType") {
                    fg->setMeshType(xml.readElementText().toUInt());
                } else if (xml.name() == "animKeyframes") { //TODO
                    Animation::TransformAnimation *transformAnim = parseTransformAnimation(xml, true);
                    fg->setTransformAnimation(transformAnim);
                    anim = transformAnim; //For later linking (So that the loop type is set in the transformAnim)
                } else if (xml.name() == "animLoopTime") { 
                    loopTime = (xml.readElementText().toFloat());
                } else if (xml.name() == "textureScroll") {
                   fg->setTextureScroll(SerializeUtils::getVec2Attributes(xml.attributes()));
                } else {
                    qWarning().noquote() << "Unrecognised tag: foregroundModel >" << xml.name();
                }

            }

            if (anim != nullptr) {
                anim->setLoopTime(loopTime);
            }

            return fg;
        }

        Scene::GroupSceneNode* XMLConfigParser::parseItemGroup(QXmlStreamReader &xml) {
            //Default name is "Group", translated
            Scene::GroupSceneNode *group = new Scene::GroupSceneNode(QCoreApplication::tr("XMLConfigParser", "Group"));

            //Some stuff that needs to be linked after parsing the entire group
            Animation::TransformAnimation *anim = nullptr;
            EnumPlaybackState initialState = EnumPlaybackState::PLAY;
            Animation::EnumLoopType loopType = Animation::EnumLoopType::LOOPING;
            float loopTime = 0.0f;

            while (!(xml.isEndElement() && xml.name() == "itemGroup")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "itemGroup > " << xml.name();

                if (xml.name() == "name") {
                    group->setName(xml.readElementText());
                } else if (xml.name() == "rotationCenter") {
                    group->setOriginPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "initialRotation") {
                    group->setOriginRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "animSeesawType") {
                    QPair<EnumAnimationSeesawType, Animation::EnumLoopType> type = parseAnimLoopType(xml);
                    group->setAnimationSeesawType(type.first);
                    loopType = type.second; //loopType will be linked with an animation later if needed
                } else if (xml.name() == "conveyorSpeed") {
                    group->setConveyorSpeed(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "seesawSensitivity") {
                    group->setSeesawSensitivity(xml.readElementText().toFloat()); //TODO: Error checking
                } else if (xml.name() == "seesawResetStiffness") { //Deprecated
                    qWarning() << "seesawResetStiffness is deprecated! Prefer seesawFriction instead";
                    group->setSeesawFriction(xml.readElementText().toFloat()); //TODO: Error checking
                } else if (xml.name() == "seesawFriction") {
                    group->setSeesawFriction(xml.readElementText().toFloat()); //TODO: Error checking
                } else if (xml.name() == "seesawRotationBounds") { //Deprecated
                    qWarning() << "seesawRotationBounds is deprecated! Prefer seesawSpring instead";
                    group->setSeesawSpring(xml.readElementText().toFloat()); //TODO: Error checking
                } else if (xml.name() == "seesawSpring") {
                    group->setSeesawSpring(xml.readElementText().toFloat()); //TODO: Error checking
                } else if (xml.name() == "textureScroll") {
                    group->setTextureScroll(SerializeUtils::getVec2Attributes(xml.attributes()));
                } else if (xml.name() == "animKeyframes") {
                    Animation::TransformAnimation *transformAnim = parseTransformAnimation(xml, false);
                    group->setTransformAnimation(transformAnim);
                    anim = transformAnim; //For later linking (So that the loop type is set in the transformAnim)
                } else if (xml.name() == "animLoopTime") {
                    loopTime = xml.readElementText().toFloat(); //For later linking
                } else if (xml.name() == "animGroupId") {
                    group->setAnimationGroupId(xml.readElementText().toUInt());
                } else if (xml.name() == "animInitialState") {
                    initialState = PlaybackState::fromString(xml.readElementText());
                } else if (xml.name() == "collisionGrid") {
                    group->setCollisionGrid(parseCollisionGrid(xml));
                } else if (xml.name() == "goal") {
                    group->addChild(parseGoal(xml));
                } else if (xml.name() == "bumper") {
                    group->addChild(parseBumper(xml));
                } else if (xml.name() == "jamabar") {
                    group->addChild(parseJamabar(xml));
                } else if (xml.name() == "banana") {
                    group->addChild(parseBanana(xml));
                } else if (xml.name() == "cone") {
                    group->addChild(parseConeCollisionObject(xml));  
                } else if (xml.name() == "sphere") {
                    group->addChild(parseSphereCollisionObject(xml));
                } else if (xml.name() == "cylinder") {
                    group->addChild(parseCylinderCollisionObject(xml));
                } else if (xml.name() == "falloutVolume") {
                    group->addChild(parseFalloutVolume(xml));
                } else if (xml.name() == "wormhole") {
                    group->addChild(parseWormhole(xml));
                } else if (xml.name() == "switch") {
                    group->addChild(parseSwitch(xml));
                } else if (xml.name() == "levelModel") { //Deprecated
                    group->addChild(parseLevelModel(xml));
                } else if (xml.name() == "stageModel") {
                    group->addChild(parseStageModel(xml));
                } else {
                    qWarning().noquote() << "Unrecognised tag: itemGroup >" << xml.name();
                }
            }

            if (anim != nullptr) {
                anim->setInitialState(initialState);
                anim->setLoopType(loopType);
                anim->setLoopTime(loopTime);
            }

            return group;
        }

        Scene::GoalSceneNode* XMLConfigParser::parseGoal(QXmlStreamReader &xml) {
            //Default name is "Goal x", translated
            static unsigned int id = 0;
            Scene::GoalSceneNode *goal = new Scene::GoalSceneNode(QCoreApplication::translate("XMLConfigParser", "Goal %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "goal")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "goal > " << xml.name();

                if (xml.name() == "name") {
                    goal->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    goal->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    goal->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "type") {
                    goal->setType(GoalType::fromString(xml.readElementText()));
                } else if (xml.name() == "castShadow") {
                    goal->setCastShadow(xml.readElementText() == "true");    
                } else {
                    qWarning().noquote() << "Unrecognised tag: goal >" << xml.name();
                }
            }

            id++;

            return goal;
        }

        Scene::BumperSceneNode* XMLConfigParser::parseBumper(QXmlStreamReader &xml) {
            //Default name is "Bumper x", translated
            static unsigned int id = 0;
            Scene::BumperSceneNode *bumper = new Scene::BumperSceneNode(QCoreApplication::translate("XMLConfigParser", "Bumper %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "bumper")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "bumper > " << xml.name();

                if (xml.name() == "name") {
                    bumper->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    bumper->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    bumper->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "scale") {
                    bumper->setScale(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else {
                    qWarning().noquote() << "Unrecognised tag: bumper >" << xml.name();
                }
            }

            id++;

            return bumper;
        }

        Scene::JamabarSceneNode* XMLConfigParser::parseJamabar(QXmlStreamReader &xml) {
            //Default name is "Jamabar x", translated
            static unsigned int id = 0;
            Scene::JamabarSceneNode *jamabar = new Scene::JamabarSceneNode(QCoreApplication::translate("XMLConfigParser", "Jamabar %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "jamabar")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "jamabar > " << xml.name();

                if (xml.name() == "name") {
                    jamabar->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    jamabar->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    jamabar->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "scale") {
                    jamabar->setScale(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else {
                    qWarning().noquote() << "Unrecognised tag: jamabar >" << xml.name();
                }
            }

            id++;

            return jamabar;
        }

        Scene::BananaSceneNode* XMLConfigParser::parseBanana(QXmlStreamReader &xml) {
            //Default name is "Banana x", translated
            static unsigned int id = 0;
            Scene::BananaSceneNode *banana = new Scene::BananaSceneNode(QCoreApplication::translate("XMLConfigParser", "Banana %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "banana")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "banana > " << xml.name();

                if (xml.name() == "name") {
                    banana->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    banana->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "type") {
                    banana->setType(BananaType::fromString(xml.readElementText()));
                } else {
                    qWarning().noquote() << "Unrecognised tag: banana >" << xml.name();
                }
            }

            id++;

            return banana;
        }

        Scene::ConeCollisionObjectSceneNode* XMLConfigParser::parseConeCollisionObject(QXmlStreamReader &xml) {
            //Default name is "Cone Collision Object x", translated
            static unsigned int id = 0;
            Scene::ConeCollisionObjectSceneNode *cone = new Scene::ConeCollisionObjectSceneNode(QCoreApplication::translate("XMLConfigParser", "Cone Collision Object %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "cone")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "cone > " << xml.name();

                if (xml.name() == "name") {
                    cone->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    cone->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    cone->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "radius") {
                    cone->setRadius(xml.readElementText().toFloat());
                } else if (xml.name() == "height") {
                    cone->setHeight(xml.readElementText().toFloat());
                } else {
                    qWarning().noquote() << "Unrecognised tag: cone >" << xml.name();
                }
            }

            id++;

            return cone;
        }

        Scene::SphereCollisionObjectSceneNode* XMLConfigParser::parseSphereCollisionObject(QXmlStreamReader &xml) {
            //Default name is "Sphere Collision Object x", translated
            static unsigned int id = 0;
            Scene::SphereCollisionObjectSceneNode *sphere = new Scene::SphereCollisionObjectSceneNode(QCoreApplication::translate("XMLConfigParser", "Sphere Collision Object %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "sphere")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "sphere > " << xml.name();

                if (xml.name() == "name") {
                    sphere->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    sphere->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "radius") {
                    sphere->setRadius(xml.readElementText().toFloat());
                } else {
                    qWarning().noquote() << "Unrecognised tag: sphere >" << xml.name();
                }
            }

            id++;

            return sphere;
        }

        Scene::CylinderCollisionObjectSceneNode* XMLConfigParser::parseCylinderCollisionObject(QXmlStreamReader &xml) {
            //Default name is "Cylinder Collision Object x", translated
            static unsigned int id = 0;
            Scene::CylinderCollisionObjectSceneNode *cylinder = new Scene::CylinderCollisionObjectSceneNode(QCoreApplication::translate("XMLConfigParser", "Cylinder Collision Object %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "cylinder")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "cylinder > " << xml.name();

                if (xml.name() == "name") {
                    cylinder->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    cylinder->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    cylinder->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "radius") {
                    cylinder->setRadius(xml.readElementText().toFloat());
                } else if (xml.name() == "height") {
                    cylinder->setHeight(xml.readElementText().toFloat());
                } else {
                    qWarning().noquote() << "Unrecognised tag: cylinder >" << xml.name();
                }
            }

            id++;

            return cylinder;
        }

        Scene::FalloutVolumeSceneNode* XMLConfigParser::parseFalloutVolume(QXmlStreamReader &xml) {
            //Default name is "Fallout Volume x", translated
            static unsigned int id = 0;
            Scene::FalloutVolumeSceneNode *volume = new Scene::FalloutVolumeSceneNode(QCoreApplication::translate("XMLConfigParser", "Fallout Volume %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "falloutVolume")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "falloutVolume > " << xml.name();

                if (xml.name() == "name") {
                    volume->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    volume->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    volume->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "scale") {
                    volume->setScale(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else {
                    qWarning().noquote() << "Unrecognised tag: falloutVolume >" << xml.name();
                }
            }

            id++;

            return volume;
        }

        Scene::WormholeSceneNode* XMLConfigParser::parseWormhole(QXmlStreamReader &xml) {
            //Default name is "Wormhole x", translated
            static unsigned int id = 0;
            Scene::WormholeSceneNode *wh = new Scene::WormholeSceneNode(QCoreApplication::translate("XMLConfigParser", "Wormhole %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "wormhole")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "wormhole > " << xml.name();

                if (xml.name() == "name") {
                    wh->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    wh->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    wh->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "destinationName") {
                    wormholeDestMap[wh] = xml.readElementText();
                } else {
                    qWarning().noquote() << "Unrecognised tag: wormhole >" << xml.name();
                }
            }

            id++;

            return wh;
        }

        Scene::SwitchSceneNode* XMLConfigParser::parseSwitch(QXmlStreamReader &xml) {
            //Default name is "Switch x", translated
            static unsigned int id = 0;
            //Woo can't use switch as a variable name!
            Scene::SwitchSceneNode *sw = new Scene::SwitchSceneNode(QCoreApplication::translate("XMLConfigParser", "Switch %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "switch")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "switch > " << xml.name();

                if (xml.name() == "name") {
                    sw->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    sw->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    sw->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else if (xml.name() == "type") {
                    sw->setType(PlaybackState::fromString(xml.readElementText()));
                } else if (xml.name() == "animGroupId") {
                    sw->setLinkedAnimGroupId(xml.readElementText().toUShort());
                } else {
                    qWarning().noquote() << "Unrecognised tag: switch >" << xml.name();
                }
            }

            id++;

            return sw;
        }

        Scene::MeshSceneNode* XMLConfigParser::parseLevelModel(QXmlStreamReader &xml) {
            QString name = xml.readElementText();

            //This is deprecated!
            qWarning() << "levelModel is deprecated! Prefer stageModel instead";
            qWarning().noquote().nospace() << "    <stageModel>";
            qWarning().noquote().nospace() << "        <name>" << name << "</name>";
            qWarning().noquote().nospace() << "        <collision>";
            qWarning().noquote().nospace() << "            <meshCollision>";
            qWarning().noquote().nospace() << "                <name>" << name << "</name>";
            qWarning().noquote().nospace() << "            </meshCollision>";
            qWarning().noquote().nospace() << "        </collision>";
            qWarning().noquote().nospace() << "    </stageModel>";

            Scene::MeshSceneNode *mesh = new Scene::MeshSceneNode(name);
            mesh->setMeshName(name);

            //Add the mesh collision, using the same object for collision
            Scene::MeshCollisionSceneNode *collision = new Scene::MeshCollisionSceneNode("Mesh Collision: " + name);
            collision->setMeshName(name);
            mesh->addChild(collision);

            return mesh;
        }

        Scene::MeshSceneNode* XMLConfigParser::parseStageModel(QXmlStreamReader &xml) {
            Scene::MeshSceneNode *mesh = new Scene::MeshSceneNode();

            while (!(xml.isEndElement() && xml.name() == "stageModel")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "stageModel > " << xml.name();

                if (xml.name() == "name") {
                    QString name = xml.readElementText();
                    mesh->setName(name);
                    mesh->setMeshName(name);
                } else if (xml.name() == "runtimeReflective") {
                    mesh->setRuntimeReflective(xml.readElementText() == "true");
                } else if (xml.name() == "collision") {
                    for (Scene::CollisionSceneNode *collision : parseCollision(xml)) mesh->addChild(collision);
                } else if (xml.name() == "bitflag") {
                    mesh->setBitflag(xml.readElementText().toUInt()); 
                } else {
                    qWarning().noquote() << "Unrecognised tag: stageModel >" << xml.name();
                }
            }

            return mesh;
        }

        QVector<Scene::CollisionSceneNode*> XMLConfigParser::parseCollision(QXmlStreamReader &xml) {
            QVector<Scene::CollisionSceneNode*> outNodes;

            while (!(xml.isEndElement() && xml.name() == "collision")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "collision > " << xml.name();

                if (xml.name() == "meshCollision") {
                    outNodes.append(parseMeshCollision(xml));
                } else {
                    qWarning().noquote() << "Unrecognised tag: collision >" << xml.name();
                }
            }

            return outNodes;
        }

        Scene::MeshCollisionSceneNode* XMLConfigParser::parseMeshCollision(QXmlStreamReader &xml) {
            Scene::MeshCollisionSceneNode *node = new Scene::MeshCollisionSceneNode();

            while (!(xml.isEndElement() && xml.name() == "meshCollision")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "meshCollision > " << xml.name();

                if (xml.name() == "name") {
                    QString name = xml.readElementText();
                    node->setName(QCoreApplication::translate("XMLConfigParser", "Mesh Collision: %1").arg(name));
                    node->setMeshName(name);
                } else if (xml.name() == "collisionFlag") {
                    node->setCollisionTriangleFlag(xml.readElementText().toShort());
                } else {
                    qWarning().noquote() << "Unrecognised tag: meshCollision >" << xml.name();
                }
            }

            return node;
        }

        CollisionGrid XMLConfigParser::parseCollisionGrid(QXmlStreamReader &xml) {
            CollisionGrid grid;

            while (!(xml.isEndElement() && xml.name() == "collisionGrid")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "collisionGrid > " << xml.name();

                if (xml.name() == "start") {
                    grid.setGridStart(SerializeUtils::getVec2Attributes(xml.attributes(), "x", "z"));
                } else if (xml.name() == "step") {
                    grid.setGridStep(SerializeUtils::getVec2Attributes(xml.attributes(), "x", "z"));
                } else if (xml.name() == "count") {
                    grid.setGridStepCount(SerializeUtils::getUVec2Attributes(xml.attributes(), "x", "z"));
                } else {
                    qWarning().noquote() << "Unrecognised tag: collisionGrid >" << xml.name();
                }
            }

            return grid;
        }

        QPair<EnumAnimationSeesawType, Animation::EnumLoopType> XMLConfigParser::parseAnimLoopType(QXmlStreamReader &xml) {
            QString str = xml.readElementText();
            EnumAnimationSeesawType animSeesawType;
            Animation::EnumLoopType loopType;

            if (str == "LOOPING_ANIMATION") {
                animSeesawType = ANIMATION;
                loopType = Animation::LOOPING;
            } else if (str == "PLAY_ONCE_ANIMATION") {
                animSeesawType = ANIMATION;
                loopType = Animation::PLAY_ONCE;
            } else if (str == "SEESAW") {
                animSeesawType = SEESAW;
            }

            return QPair<EnumAnimationSeesawType, Animation::EnumLoopType>(animSeesawType, loopType);
        }

        Animation::TransformAnimation* XMLConfigParser::parseTransformAnimation(QXmlStreamReader &xml, bool supportsScale) {
            Animation::TransformAnimation *anim = new Animation::TransformAnimation;

            while (!(xml.isEndElement() && xml.name() == "animKeyframes")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "animKeyframes > " << xml.name();

                if (xml.name() == "posX") {
                    parseKeyframes(xml, anim->getPosXKeyframes());
                } else if (xml.name() == "posY") {
                    parseKeyframes(xml, anim->getPosYKeyframes());
                } else if (xml.name() == "posZ") {
                    parseKeyframes(xml, anim->getPosZKeyframes());
                } else if (xml.name() == "rotX") {
                    parseKeyframes(xml, anim->getRotXKeyframes(), true);
                } else if (xml.name() == "rotY") {
                    parseKeyframes(xml, anim->getRotYKeyframes(), true);
                } else if (xml.name() == "rotZ") {
                    parseKeyframes(xml, anim->getRotZKeyframes(), true);
                } else if (xml.name() == "scaleX" && supportsScale) {
                    parseKeyframes(xml, anim->getScaleXKeyframes()); 
                } else if (xml.name() == "scaleY" && supportsScale) {
                    parseKeyframes(xml, anim->getScaleYKeyframes()); 
                } else if (xml.name() == "scaleZ" && supportsScale) {
                    parseKeyframes(xml, anim->getScaleZKeyframes()); 
                } else {
                    qWarning().noquote() << "Unrecognised tag: animKeyframes >" << xml.name();
                }
            }

            return anim;
        }

        Fog* XMLConfigParser::parseFog(QXmlStreamReader &xml)
        {
            Fog* fog = new Fog();

            while (!(xml.isEndElement() && xml.name() == "fog")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                "fog > " << xml.name();

                if (xml.name() == "type") {
                    fog->setFogType(FogType::fromString(xml.readElementText()));
                } else if (xml.name() == "red") {
                    fog->setRedValue(xml.readElementText().toFloat());
                } else if (xml.name() == "green") {
                    fog->setGreenValue(xml.readElementText().toFloat());
                } else if (xml.name() == "blue") {
                    fog->setBlueValue(xml.readElementText().toFloat());
                } else if (xml.name() == "start") {
                    fog->setStartDistance(xml.readElementText().toFloat());
                } else if (xml.name() == "end") {
                    fog->setEndDistance(xml.readElementText().toFloat());
                } else {
                    qWarning().noquote() << "Unrecgonised tag: fog > " << xml.name();
                }
            }

            return fog;

        }

        Animation::FogAnimation* XMLConfigParser::parseFogAnimation(QXmlStreamReader &xml)
        {
            Animation::FogAnimation* anim = new Animation::FogAnimation;

            while (!(xml.isEndElement() && xml.name() == "fogAnimationKeyframes")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "fogAnimationKeyframes > " << xml.name();

                if (xml.name() == "red") {
                    parseKeyframes(xml, anim->getRedKeyframes());
                } else if (xml.name() == "green") {
                    parseKeyframes(xml, anim->getGreenKeyframes());
                } else if (xml.name() == "blue") {
                    parseKeyframes(xml, anim->getBlueKeyframes());
                } else if (xml.name() == "start") {
                    parseKeyframes(xml, anim->getStartDistanceKeyframes(), true);
                } else if (xml.name() == "end") {
                    parseKeyframes(xml, anim->getEndDistanceKeyframes(), true);
                } else if (xml.name() == "unknown") {
                    parseKeyframes(xml, anim->getUnknownKeyframes(), true);
                } else {
                    qWarning().noquote() << "Unrecognised tag: fogAnimationKeyframes >" << xml.name();
                }
           }

           return anim;

        }

        Scene::BoosterSceneNode *XMLConfigParser::parseBooster(QXmlStreamReader &xml)
        {
            //Default name is "Booster x", translated
            static unsigned int id = 0;
            Scene::BoosterSceneNode *booster = new Scene::BoosterSceneNode(QCoreApplication::translate("XMLConfigParser", "Booster %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "booster")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "booster > " << xml.name();

                if (xml.name() == "name") {
                    booster->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    booster->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    booster->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else {
                    qWarning().noquote() << "Unrecognised tag: booster >" << xml.name();
                }
            }

            id++;

            return booster;
        }

        Scene::GolfHoleSceneNode *XMLConfigParser::parseGolfHole(QXmlStreamReader &xml)
        {

            //Default name is "GolfHole x", translated
            static unsigned int id = 0;
            Scene::GolfHoleSceneNode *golfHole = new Scene::GolfHoleSceneNode(QCoreApplication::translate("XMLConfigParser", "GolfHole %1").arg(id));

            while (!(xml.isEndElement() && xml.name() == "golfHole")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "golfHole > " << xml.name();

                if (xml.name() == "name") {
                    golfHole->setName(xml.readElementText());
                } else if (xml.name() == "position") {
                    golfHole->setPosition(SerializeUtils::getVec3Attributes(xml.attributes()));
                } else if (xml.name() == "rotation") {
                    golfHole->setRotation(MathUtils::degreesToRadians(SerializeUtils::getVec3Attributes(xml.attributes())));
                } else {
                    qWarning().noquote() << "Unrecognised tag: golfHole >" << xml.name();
                }
            }

            id++;

            return golfHole;
        }

       Scene::RaceTrackPathSceneNode* XMLConfigParser::parseTrackPath(QXmlStreamReader &xml)
        {
            static unsigned int id = 0;
            Scene::RaceTrackPathSceneNode *node = new Scene::RaceTrackPathSceneNode(QCoreApplication::translate("XMLConfigParser", "RaceTrackPath %1").arg(id));
            Animation::RaceTrackPath *trackPath = new Animation::RaceTrackPath();
            node->setTrackPath(trackPath);

            while (!(xml.isEndElement() && xml.name() == "trackPath")) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    "trackPath > " << xml.name();

                if (xml.name() == "posX") {
                    parseKeyframes(xml, node->getTrackPath()->getPosXKeyframes());
                } else if (xml.name() == "posY") {
                    parseKeyframes(xml, node->getTrackPath()->getPosYKeyframes());
                } else if (xml.name() == "posZ") {
                    parseKeyframes(xml, node->getTrackPath()->getPosZKeyframes());
                } else if (xml.name() == "playerID") {
                    node->getTrackPath()->setPlayerID(xml.readElementText().toUInt());
                } else {
                    qWarning().noquote() << "Unrecognised tag: trackPath >" << xml.name();
                }
            }

            id++;
            return node;
        }

        void XMLConfigParser::parseKeyframes(
                QXmlStreamReader &xml,
                std::set<Animation::KeyframeF*, Animation::KeyframeCompare> &keyframes,
                bool convertToRadians
                ) {
            using namespace Animation;

            QStringRef parentName = xml.name();

            while (!(xml.isEndElement() && xml.name() == parentName)) {
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                qDebug().nospace() << "XML config parsing [Line: " << xml.lineNumber() << ", Col: " << xml.columnNumber() << "]: " <<
                    parentName << " > " << xml.name();

                if (xml.name() == "keyframe") {
                    float time;
                    float value;
                    EnumEasing easing;

                    //Search through the attributes for time, value, and easing
                    foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                        if (attr.name() == "time") {
                            time = attr.value().toFloat();
                        } else if (attr.name() == "value") {
                            value = attr.value().toFloat();

                            if (convertToRadians) value = qDegreesToRadians(value);
                        } else if (attr.name() == "easing") {
                            easing = Easing::fromString(attr.value().toString());
                        }
                    }

                    //Create the keyframe
                    KeyframeF *k = new KeyframeF(time, value, easing);
                    keyframes.insert(k);
                } else {
                    qWarning().noquote() << "Unrecognised tag: [keyframeIdentier] >" << xml.name();
                }
            }
        }
    }
}
