#include "MathUtils.hpp"

namespace WS2 {
    namespace MathUtils {
        glm::vec3 toGlmVec3(QVector3D *vec) {
            return glm::vec3(vec->x(), vec->y(), vec->z());
        }

        glm::vec3 toGlmVec3(QVector2D *vec) {
            return glm::vec3(vec->x(), vec->y(), 0.0f);
        }

        glm::vec2 toGlmVec2(QVector2D *vec) {
            return glm::vec2(vec->x(), vec->y());
        }
    }
}

