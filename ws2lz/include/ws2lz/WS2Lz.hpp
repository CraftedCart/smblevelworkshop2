/**
 * @file
 * @brief Misc functions for WS2Lz
 */

#include "ws2lz_export.h"
#include <QDataStream>
#include <glm/glm.hpp>

WS2LZ_EXPORT QDataStream& operator<<(QDataStream& dev, const glm::vec3& vec);
WS2LZ_EXPORT QDataStream& operator<<(QDataStream& dev, const glm::tvec3<quint16>& vec);
WS2LZ_EXPORT QDataStream& operator<<(QDataStream& dev, const glm::vec2& vec);
WS2LZ_EXPORT QDataStream& operator<<(QDataStream& dev, const glm::uvec2& vec);

