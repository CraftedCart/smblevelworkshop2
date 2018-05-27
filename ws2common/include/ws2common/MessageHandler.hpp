/**
 * @file
 * @brief Header for the message handler, for logging output
 */

#include "ws2common/scene/SceneNode.hpp"
#include <QtGlobal>
#include <QString>
#include <QDebug>
#include <glm/glm.hpp>

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_MESSAGEHANDLER_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_MESSAGEHANDLER_HPP

namespace WS2Common {
    WS2COMMON_EXPORT void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}

//Custom types
WS2COMMON_EXPORT QDebug operator<<(QDebug debug, const glm::vec3 &vec);
WS2COMMON_EXPORT QDebug operator<<(QDebug debug, const glm::vec2 &vec);
WS2COMMON_EXPORT QDebug operator<<(QDebug debug, const glm::uvec2 &vec);
WS2COMMON_EXPORT QDebug operator<<(QDebug debug, const WS2Common::Scene::SceneNode *node);

#endif

