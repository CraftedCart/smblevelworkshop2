/**
 * @file
 * @brief Template function implementation file for the PhysicsUtils namespace
 */

#include <LinearMath/btAlignedObjectArray.h>

namespace WS2Editor {
    namespace Physics {
        namespace PhysicsUtils {
            template <typename T>
            bool doesAlignedObjectArrayContain(const btAlignedObjectArray<T> &arr, const T &obj) {
                for (int i = 0; i < arr.size(); i++) {
                    if (arr.at(i) == obj) return true;
                }

                return false;
            }
        }
    }
}

