#ifndef SMBLEVELWORKSHOP2_MATHUTILS_HPP
#define SMBLEVELWORKSHOP2_MATHUTILS_HPP

#include <glm/glm.hpp>
#include <QVector3D>
#include <QVector2D>
#include <assimp/matrix4x4.h>

namespace WS2 {
    namespace MathUtils {
        glm::vec3 toGlmVec3(QVector3D &vec);
        glm::vec3 toGlmVec3(QVector2D &vec);
        glm::vec2 toGlmVec2(QVector2D &vec);
        glm::mat4 toGlmMat4(aiMatrix4x4 &mat);
    }
}

#endif

