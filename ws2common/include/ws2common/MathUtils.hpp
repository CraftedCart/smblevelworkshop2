/**
 * @file
 * @brief Header for the MathUtils namespace, containing various math related helper functions
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_MATHUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_MATHUTILS_HPP

#include "ws2common_export.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <QVector3D>
#include <QVector2D>
#include <assimp/matrix4x4.h>

namespace WS2Common {

    /**
     * @brief Various functions to convert between types and other misc math related functions
     */
    namespace MathUtils {
        WS2COMMON_EXPORT glm::vec3 toGlmVec3(const QVector3D &vec);
        WS2COMMON_EXPORT glm::vec3 toGlmVec3(const QVector2D &vec);
        WS2COMMON_EXPORT glm::vec2 toGlmVec2(const QVector2D &vec);
        WS2COMMON_EXPORT glm::mat4 toGlmMat4(const aiMatrix4x4 &mat);

        /**
         * @brief Converts euler angles to a quaternion in the order Z, Y, X
         *
         * @param euler The euler angles
         *
         * @return The quaternion from the euler angles
         */
        WS2COMMON_EXPORT glm::quat eulerZyxToGlmQuat(const glm::vec3 &euler);

        WS2COMMON_EXPORT QPoint toQPoint(const glm::vec2 &vec);
        WS2COMMON_EXPORT int randInt(const int a, const int b);
    }
}

#endif

