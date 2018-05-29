/**
 * @file
 * @brief Header for the SerializeUtils function
 *        This contains various utilities to serialize/deserialize data
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SERIALIZEUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SERIALIZEUTILS_HPP

#include "ws2common_export.h"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include <glm/glm.hpp>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace WS2Common {
    namespace SerializeUtils {
        WS2COMMON_EXPORT Scene::SceneNode* deserializeNodeFromXml(QXmlStreamReader &xml, Scene::SceneNode *parent = nullptr);

        WS2COMMON_EXPORT void parseSceneNode(QXmlStreamReader &xml, Scene::SceneNode *node);
        WS2COMMON_EXPORT void parseSceneNodeData(QXmlStreamReader &xml, Scene::SceneNode *node);
        WS2COMMON_EXPORT void parseMeshSceneNode(QXmlStreamReader &xml, Scene::MeshSceneNode *node);
        WS2COMMON_EXPORT void parseMeshSceneNodeData(QXmlStreamReader &xml, Scene::MeshSceneNode *node);
        WS2COMMON_EXPORT void parseGroupSceneNode(QXmlStreamReader &xml, Scene::GroupSceneNode *node);
        WS2COMMON_EXPORT void parseGroupSceneNodeData(QXmlStreamReader &xml, Scene::GroupSceneNode *node);
        WS2COMMON_EXPORT void parseGoalSceneNode(QXmlStreamReader &xml, Scene::GoalSceneNode *node);
        WS2COMMON_EXPORT void parseGoalSceneNodeData(QXmlStreamReader &xml, Scene::GoalSceneNode *node);
        WS2COMMON_EXPORT void parseBumperSceneNode(QXmlStreamReader &xml, Scene::BumperSceneNode *node);
        //Bumper scene node has no additional data to parse
        WS2COMMON_EXPORT void parseBananaSceneNode(QXmlStreamReader &xml, Scene::BananaSceneNode *node);
        WS2COMMON_EXPORT void parseBananaSceneNodeData(QXmlStreamReader &xml, Scene::BananaSceneNode *node);
        WS2COMMON_EXPORT void parseJamabarSceneNode(QXmlStreamReader &xml, Scene::JamabarSceneNode *node);
        //Jamabar scene node has no additional data to parse

        WS2COMMON_EXPORT void parseChildren(QXmlStreamReader &xml, Scene::SceneNode *node);

        WS2COMMON_EXPORT void writeVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::vec2 vec,
                QString xAttrName = "x",
                QString yAttrName = "y"
                );

        WS2COMMON_EXPORT void writeUVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::uvec2 vec,
                QString xAttrName = "x",
                QString yAttrName = "y"
                );

        WS2COMMON_EXPORT void writeVec3(
                QXmlStreamWriter &xml,
                QString name,
                glm::vec3 vec,
                QString xAttrName = "x",
                QString yAttrName = "y",
                QString zAttrName = "z"
                );

        /**
         * @brief Gets an XML attribute with the name attrName
         *
         * @param attrs The attributes to search through
         * @param attrName The name of the attribute to fetch
         *
         * @return The value of the attribute
         */
        WS2COMMON_EXPORT QStringRef getAttribute(const QXmlStreamAttributes &attrs, const QString attrName);

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
        WS2COMMON_EXPORT glm::vec3 getVec3Attributes(const QXmlStreamAttributes &attrs,
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
        WS2COMMON_EXPORT glm::vec2 getVec2Attributes(const QXmlStreamAttributes &attrs,
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
        WS2COMMON_EXPORT glm::uvec2 getUVec2Attributes(const QXmlStreamAttributes &attrs,
                const QString x = "x", const QString y = "y");
    }
}

#endif

