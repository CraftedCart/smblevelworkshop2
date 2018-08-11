/**
 * @file
 * @brief Header for the StagedefConfigParser class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_CONFIG_STAGEDEFCONFIGPARSER_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_CONFIG_STAGEDEFCONFIGPARSER_HPP

#include "ws2common/Stage.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include <QByteArray>
#include <QDataStream>

namespace WS2Common {
    namespace Config {
        class WS2COMMON_EXPORT StagedefConfigParser {
            public:
                /**
                 * @brief Parses a raw LZ config and converts it into a Stage object
                 *
                 * @param bytes The stagedef data to parse
                 *
                 * @return A pointer to stage created
                 */
                Stage* parseStage(QByteArray &bytes);

            protected:
                void parseSmb1Stagedef(Stage *stage, QByteArray &bytes);
                Scene::GroupSceneNode* parseSmb1CollisionGroup(QByteArray &bytes, quint32 offset);
                Scene::GoalSceneNode* parseSmb1Goal(QByteArray &bytes, quint32 offset);

                Scene::StartSceneNode* parseStart(QByteArray &bytes, quint32 offset);
                Scene::BumperSceneNode* parseBumper(QByteArray &bytes, quint32 offset);
                Scene::JamabarSceneNode* parseJamabar(QByteArray &bytes, quint32 offset);
                Scene::BananaSceneNode* parseBanana(QByteArray &bytes, quint32 offset);
                Scene::MeshSceneNode* parseLevelModel(QByteArray &bytes, quint32 offset);

                glm::vec3 rotationShortToRadians(glm::tvec3<quint16> vec);

                void configureDataStream(QDataStream &stream);
        };
    }
}

#endif

