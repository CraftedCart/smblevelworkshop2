/**
 * @file
 * @brief Header for the MathUtils namespace, containing various math related helper functions
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_MATHUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_MATHUTILS_HPP

#include <glm/glm.hpp>
#include <QVector3D>
#include <QVector2D>
#include <assimp/matrix4x4.h>

namespace WS2Common {

    /**
     * @brief Various functions to convert between types and other misc math related functions
     */
    namespace MathUtils {
        glm::vec3 toGlmVec3(const QVector3D &vec);
        glm::vec3 toGlmVec3(const QVector2D &vec);
        glm::vec2 toGlmVec2(const QVector2D &vec);
        glm::mat4 toGlmMat4(const aiMatrix4x4 &mat);
        QPoint toQPoint(const glm::vec2 &vec);
        int randInt(const int a, const int b);
    }
}

#endif

