/**
 * @file
 * @brief Header for the SerializeUtils function
 *        This contains various utilities to serialize/deserialize data
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SERIALIZEUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SERIALIZEUTILS_HPP

#include "ws2common/scene/MeshSceneNode.hpp"
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

        void parseChildren(QXmlStreamReader &xml, SceneNode *node);

        void writeVec2(
                QXmlStreamWriter &xml,
                QString name,
                glm::vec2 vec,
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
    }
}

#endif

