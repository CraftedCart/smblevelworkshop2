/**
 * @file
 * @brief Header for the PhysicsUtils namespace
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PHYSICS_PHYSICSUTILS_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PHYSICS_PHYSICSUTILS_HPP

#include "ws2editor_export.h"
#include <LinearMath/btAlignedObjectArray.h>

namespace WS2Editor {
    namespace Physics {
        namespace PhysicsUtils {
            template <typename T>
            WS2EDITOR_EXPORT bool doesAlignedObjectArrayContain(const btAlignedObjectArray<T> &arr, const T &obj);
        }
    }
}

#include "ws2editor/physics/PhysicsUtils.ipp"

#endif

