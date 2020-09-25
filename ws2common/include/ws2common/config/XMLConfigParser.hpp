/**
 * @file
 * @brief Header for the XMLConfigParser class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_CONFIG_XMLCONFIGPARSER_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_CONFIG_XMLCONFIGPARSER_HPP

#include "ws2common/Stage.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common_export.h"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/ConeCollisionObjectSceneNode.hpp"
#include "ws2common/scene/SphereCollisionObjectSceneNode.hpp"
#include "ws2common/scene/CylinderCollisionObjectSceneNode.hpp"
#include "ws2common/scene/FalloutVolumeSceneNode.hpp"
#include "ws2common/scene/SwitchSceneNode.hpp"
#include "ws2common/scene/WormholeSceneNode.hpp"
#include "ws2common/scene/MeshCollisionSceneNode.hpp"
#include "ws2common/scene/RaceTrackPathSceneNode.hpp"
#include "ws2common/scene/BoosterSceneNode.hpp"
#include "ws2common/scene/GolfHoleSceneNode.hpp"
#include "ws2common/animation/RaceTrackPath.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/EnumAnimationSeesawType.hpp"
#include "ws2common/CollisionGrid.hpp"
#include <QXmlStreamAttributes>
#include <QHash>
#include <QUrl>
#include <QDir>

namespace WS2Common {
    namespace Config {
        class WS2COMMON_EXPORT XMLConfigParser {
            protected:
                QHash<Scene::WormholeSceneNode*, QString> wormholeDestMap;

            public:
                /**
                 * @brief Parses an XML config and converts it into a Stage object
                 *
                 * @param config The XML as a string to parse
                 * @param relativeRoot The directory the config file resides in, to allow obtaining paths for relative files
                 *
                 * @return A pointer to stage created
                 */
                Stage* parseStage(QString config, QDir relativeRoot);

            protected:
                /**
                 * @brief Parses a model import of an XML config
                 *
                 * Make sure the XML reader is within the start element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 * @param relativeRoot The directory the config file resides in, to allow obtaining paths for relative files
                 *
                 * @return The URL containing the path to the model requested, or an empty URL if the file doesn't exist
                 */
                QUrl parseModelImport(QXmlStreamReader &xml, QDir relativeRoot);

                /**
                 * @brief Parses the start section of an XML config
                 *
                 * Make sure the XML reader is within the start element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The start parsed from the config
                 */
                Scene::StartSceneNode* parseStart(QXmlStreamReader &xml);

                /**
                 * @brief Parses a background model in an XML config
                 *
                 * Make sure the XML reader is within the backgroundModel element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The background model parsed from the config
                 */
                Scene::MeshSceneNode* parseBackgroundModel(QXmlStreamReader &xml);

                /**
                 * @brief Parses a background model in an XML config
                 *
                 * Make sure the XML reader is within the backgroundModel element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The background model parsed from the config
                 */
                Scene::MeshSceneNode* parseForegroundModel(QXmlStreamReader &xml);

                /**
                 * @brief Parses an item group in an XML config
                 *
                 * Make sure the XML reader is within the itemGroup element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The item group parsed from the config, as a GroupSceneNode
                 */
                Scene::GroupSceneNode* parseItemGroup(QXmlStreamReader &xml);

                /**
                 * @brief Parses a goal in an XML config
                 *
                 * Make sure the XML reader is within the goal element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The goal parsed from the config
                 */
                Scene::GoalSceneNode* parseGoal(QXmlStreamReader &xml);

                /**
                 * @brief Parses a bumper in an XML config
                 *
                 * Make sure the XML reader is within the bumper element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The bumper parsed from the config
                 */
                Scene::BumperSceneNode* parseBumper(QXmlStreamReader &xml);

                /**
                 * @brief Parses a jamabar in an XML config
                 *
                 * Make sure the XML reader is within the jamabar element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The jamabar parsed from the config
                 */
                Scene::JamabarSceneNode* parseJamabar(QXmlStreamReader &xml);

                /**
                 * @brief Parses a banana in an XML config
                 *
                 * Make sure the XML reader is within the banana element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The banana parsed from the config
                 */
                Scene::BananaSceneNode* parseBanana(QXmlStreamReader &xml);

                /**
                 * @brief Parses a cone collision object in an XML config
                 *
                 * Make sure the XML reader is within the cone collision object element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The cone collision object parsed from the config
                 */
                Scene::ConeCollisionObjectSceneNode* parseConeCollisionObject(QXmlStreamReader &xml);

                /**
                 * @brief Parses a sphere collision object in an XML config
                 *
                 * Make sure the XML reader is within the sphere collision object element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The sphere collision object parsed from the config
                 */
                Scene::SphereCollisionObjectSceneNode* parseSphereCollisionObject(QXmlStreamReader &xml);

                /**
                 * @brief Parses a cylinder collision object in an XML config
                 *
                 * Make sure the XML reader is within the cylinder collision object element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The cylinder collision object parsed from the config
                 */
                Scene::CylinderCollisionObjectSceneNode* parseCylinderCollisionObject(QXmlStreamReader &xml);

                /**
                 * @brief Parses a fallout volume in an XML config
                 *
                 * Make sure the XML reader is within the falloutVolume element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The fallout volume parsed from the config
                 */
                Scene::FalloutVolumeSceneNode* parseFalloutVolume(QXmlStreamReader &xml);

                /**
                 * @brief Parses a wormhole in an XML config
                 *
                 * Make sure the XML reader is within the switch element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The wormhole parsed from the config
                 */
                Scene::WormholeSceneNode* parseWormhole(QXmlStreamReader &xml);

                /**
                 * @brief Parses a switch in an XML config
                 *
                 * Make sure the XML reader is within the switch element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The switch parsed from the config
                 */
                Scene::SwitchSceneNode* parseSwitch(QXmlStreamReader &xml);

                /**
                 * @brief Parses a level model in an XML config
                 * @deprecated Prefer stageModel over levelModel in a config
                 *
                 * Make sure the XML reader is within the levelModel element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The level model parsed from the config
                 */
                Scene::MeshSceneNode* parseLevelModel(QXmlStreamReader &xml);

                /**
                 * @brief Parses a stage model in an XML config
                 *
                 * Make sure the XML reader is within the stageModel element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The stage model parsed from the config
                 */
                Scene::MeshSceneNode* parseStageModel(QXmlStreamReader &xml);

                /**
                 * @brief Parses a collision block in an XML config
                 *
                 * Make sure the XML reader is within the collision element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return A vector of collision objects to attach to a node
                 */
                QVector<Scene::CollisionSceneNode*> parseCollision(QXmlStreamReader &xml);

                /**
                 * @brief Parses a mesh collision block in an XML config
                 *
                 * Make sure the XML reader is within the meshCollision element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The mesh collision parsed from the config
                 */
                Scene::MeshCollisionSceneNode* parseMeshCollision(QXmlStreamReader &xml);

                /**
                 * @brief Parses collision information in an XML config
                 *
                 * Make sure the XML reader is within the collisionGrid element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The collision parsed from the config
                 */
                CollisionGrid parseCollisionGrid(QXmlStreamReader &xml);

                /**
                 * @brief Parses the animation/seesaw type in an XML config
                 *
                 * Make sure the XML reader is within the animSeesawType element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The type parsed from the config
                 */
                QPair<EnumAnimationSeesawType, Animation::EnumLoopType> parseAnimLoopType(QXmlStreamReader &xml);

                /**
                 * @brief Parses transform animation data in an XML config
                 *
                 * Make sure the XML reader is within the animKeyframes element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The TransformAnimation parsed from the config
                 */
                Animation::TransformAnimation* parseTransformAnimation(QXmlStreamReader &xml, bool supportsScale);

                /**
                 * @brief Parses static fog data in an XML config
                 * @param xml
                 * @return The fog data parsed from the config
                 */
                Fog* parseFog(QXmlStreamReader &xml);

                /**
                 * @brief Parses animated fog data in an XML config
                 * @param xml The QXmlStreamReader
                 * @return The fog animation data parsed from the config
                 */
                Animation::FogAnimation* parseFogAnimation(QXmlStreamReader &xml);

                /**
                 * @brief Parses a booster in the XML config
                 * @param xml The QXmlStreamReader
                 * @return The booster parsed from the config
                 */
                Scene::BoosterSceneNode* parseBooster(QXmlStreamReader &xml);

                /**
                 * @brief Parses a golf hole in the XML config
                 * @param xml The QXmlStreamReader
                 * @return The golf hole parsed from the config
                 */
                Scene::GolfHoleSceneNode* parseGolfHole(QXmlStreamReader &xml);

                /**
                 * @brief Parses a track path in the XML config
                 * @param xml The QXmlStreamReader
                 * @return The track path parsed from the config
                 */
                Scene::RaceTrackPathSceneNode* parseTrackPath(QXmlStreamReader &xml);

                /**
                 * @brief Parses effect animation data in an XML config
                 * @param xml The QXmlStreamReader
                 * @return The effect animation parsed from the config
                 */
                Animation::EffectAnimation* parseEffectAnimation(QXmlStreamReader &xml);

                void parseEffectKeyframesType1(QXmlStreamReader &xml, QVector<Animation::KeyframeEffect1*> &keyframes);
                void parseEffectKeyframesType2(QXmlStreamReader &xml, QVector<Animation::KeyframeEffect2*> &keyframes);

                /**
                 * @brief Populates the keyframe set with parsed keyframes
                 *
                 * @param xml The QXmlStreamReader
                 * @param keyframes The keyframe map to populate
                 * @param convertToRadians Whether keyframe values should be interpreted as degrees and converted to
                 *                         radians first
                 */
                void parseKeyframes(
                        QXmlStreamReader &xml,
                        std::set<Animation::KeyframeF*, Animation::KeyframeCompare> &keyframes,
                        bool convertToRadians = false
                        );
        };
    }
}

#endif

