/**
 * @file
 * @brief Header for the XMLConfigParser class
 */

#include "ws2common/Stage.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/FalloutVolumeSceneNode.hpp"
#include "ws2common/scene/SwitchSceneNode.hpp"
#include "ws2common/scene/WormholeSceneNode.hpp"
#include "ws2common/CollisionGrid.hpp"
#include <QXmlStreamAttributes>
#include <QHash>
#include <QUrl>
#include <QDir>

namespace WS2Common {
    namespace Config {
        class XMLConfigParser {
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
                 * @brief Parses a fallout volume in an XML config
                 *
                 * Make sure the XML reader is within the falloutVolume element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The jamabar parsed from the config
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
                 * @brief Parses collision information in an XML config
                 *
                 * Make sure the XML reader is within the collisionGrid element before calling this
                 *
                 * @param xml The QXmlStreamReader
                 *
                 * @return The collision parsed from the config
                 */
                CollisionGrid* parseCollisionGrid(QXmlStreamReader &xml);

                /**
                 * @brief Gets an XML attribute with the name attrName
                 *
                 * @param attrs The attributes to search through
                 * @param attrName The name of the attribute to fetch
                 *
                 * @return The value of the attribute
                 */
                QStringRef getAttribute(const QXmlStreamAttributes &attrs, const QString attrName);

                /**
                 * @brief Gets 3 XML attributes with the names x, y, z, and puts them in a glm::vec3
                 *
                 * @param attrs The attributes to search through
                 * @param x The X attribute name
                 * @param y The Y attribute name
                 * @param z The Z attribute name
                 *
                 * @return A vec3 of the float values read from the x, y, z attrubutes
                 */
                glm::vec3 getVec3Attributes(const QXmlStreamAttributes &attrs,
                        const QString x = "x", const QString y = "y", const QString z = "z");

                /**
                 * @brief Gets 2 XML attributes with the names x, y and puts them in a glm::vec2
                 *
                 * @param attrs The attributes to search through
                 * @param x The X attribute name
                 * @param y The Y attribute name
                 *
                 * @return A vec2 of the float values read from the x, y attrubutes
                 */
                glm::vec2 getVec2Attributes(const QXmlStreamAttributes &attrs,
                        const QString x = "x", const QString y = "y");

                /**
                 * @brief Gets 2 XML attributes with the names x, y and puts them in a glm::uvec2
                 *
                 * @param attrs The attributes to search through
                 * @param x The X attribute name
                 * @param y The Y attribute name
                 *
                 * @return A uvec2 of the float values read from the x, y attrubutes
                 */
                glm::uvec2 getUvec2Attributes(const QXmlStreamAttributes &attrs,
                        const QString x = "x", const QString y = "y");
        };
    }
}

