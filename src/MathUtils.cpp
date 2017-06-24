#include "MathUtils.hpp"
#include <QDebug>

namespace WS2 {
    namespace MathUtils {
        glm::vec3 toGlmVec3(QVector3D &vec) {
            return glm::vec3(vec.x(), vec.y(), vec.z());
        }

        glm::vec3 toGlmVec3(QVector2D &vec) {
            return glm::vec3(vec.x(), vec.y(), 0.0f);
        }

        glm::vec2 toGlmVec2(QVector2D &vec) {
            return glm::vec2(vec.x(), vec.y());
        }

        glm::mat4 toGlmMat4(aiMatrix4x4 &mat) {
            return glm::mat4(
                    mat.a1, mat.a2, mat.a3, mat.a4,
                    mat.b1, mat.b2, mat.b3, mat.b4,
                    mat.c1, mat.c2, mat.c3, mat.c4,
                    mat.d1, mat.d2, mat.d3, mat.d4
                    );
        }
    }
}

