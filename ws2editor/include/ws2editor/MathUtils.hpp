/**
 * @file
 * @brief Header for the MathUtils namespace, containing various math related helper functions
 *
 * This header just appends to the existing MathUtils functions in WS2Common
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MATHUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MATHUTILS_HPP

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace WS2Common {
    namespace MathUtils {
        glm::vec3 toGlmVec3(const btVector3 &vec);
        btVector3 toBtVector3(const glm::vec3 &vec);
    }
}

#endif

