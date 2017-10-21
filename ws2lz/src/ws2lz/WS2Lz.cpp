#include "ws2lz/WS2Lz.hpp"

QDataStream& operator<<(QDataStream& dev, const glm::vec3& vec) {
    dev << vec.x << vec.y << vec.z;
    return dev;
}

QDataStream& operator<<(QDataStream& dev, const glm::tvec3<quint16>& vec) {
    dev << vec.x << vec.y << vec.z;
    return dev;
}

QDataStream& operator<<(QDataStream& dev, const glm::vec2& vec) {
    dev << vec.x << vec.y;
    return dev;
}

QDataStream& operator<<(QDataStream& dev, const glm::uvec2& vec) {
    dev << (quint32) vec.x << (quint32) vec.y;
    return dev;
}

