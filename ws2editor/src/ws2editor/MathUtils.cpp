#include "ws2editor/MathUtils.hpp"

namespace WS2Common {
    namespace MathUtils {
        glm::vec3 toGlmVec3(const btVector3 &vec) {
            return glm::vec3(vec.getX(), vec.getY(), vec.getZ());
        }
        btVector3 toBtVector3(const glm::vec3 &vec) {
            return btVector3(vec.x, vec.y, vec.z);
        }
    }
}

