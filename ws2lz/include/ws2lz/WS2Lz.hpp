/**
 * @file
 * @brief Misc functions for WS2Lz
 */

#include <QDataStream>
#include <glm/glm.hpp>

QDataStream& operator<<(QDataStream& dev, const glm::vec3& vec);
QDataStream& operator<<(QDataStream& dev, const glm::tvec3<quint16>& vec);

