/**
 * @file
 * @brief Header for the XMLConfigParser namespace
 */

#include "ws2common/Stage.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include <QXmlStreamAttributes>

namespace WS2Common {
    namespace Config {
        namespace XMLConfigParser {

            /**
             * @brief Parses and XML config and converts it into a Stage object
             *
             * @param config The XML as a string to parse
             *
             * @return A pointer to stage created
             */
            Stage* parseStage(QString config);

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
        }
    }
}

