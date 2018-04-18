/**
 * @file
 * @brief Header for the SerializeUtils function
 *        This contains various utilities to serialize/deserialize data
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SERIALIZEUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SERIALIZEUTILS_HPP

#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include <glm/glm.hpp>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace WS2Common {
    namespace SerializeUtils {
        using namespace WS2Common::Scene;

        SceneNode* deserializeNodeFromXml(QXmlStreamReader &xml, SceneNode *parent = nullptr);

        void parseSceneNode(QXmlStreamReader &xml, SceneNode *node);
        void parseSceneNodeData(QXmlStreamReader &xml, SceneNode *node);
        void parseMeshSceneNode(QXmlStreamReader &xml, MeshSceneNode *node);
        void parseMeshSceneNodeData(QXmlStreamReader &xml, MeshSceneNode *node);
        void parseGroupSceneNode(QXmlStreamReader &xml, GroupSceneNode *node);
        void parseGroupSceneNodeData(QXmlStreamReader &xml, GroupSceneNode *node);
        void parseGoalSceneNode(QXmlStreamReader &xml, GoalSceneNode *node);
        void parseGoalSceneNodeData(QXmlStreamReader &xml, GoalSceneNode *node);

        void parseChildren(QXmlStreamReader &xml, SceneNode *node);

        void writeVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::vec2 vec,
                QString xAttrName = "x",
                QString yAttrName = "y"
                );

        void writeUVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::uvec2 vec,
                QString xAttrName = "x",
                QString yAttrName = "y"
                );

        void writeVec3(
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
        glm::uvec2 getUVec2Attributes(const QXmlStreamAttributes &attrs,
                const QString x = "x", const QString y = "y");
    }
}

#endif

