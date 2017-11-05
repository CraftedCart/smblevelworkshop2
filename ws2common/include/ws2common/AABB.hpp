/**
 * @file
 * @brief Header for the AABB (Axis-Aligned Bounding Box) structs
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_AABB_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_AABB_HPP

#include <glm/glm.hpp>

namespace WS2Common {
    struct AABB2 {
        glm::vec2 a;
        glm::vec2 b;

        AABB2(glm::vec2 a, glm::vec2 b) {
            this->a = a;
            this->b = b;
        }

        float getX() { return a.x; }
        float getY() { return a.y; }
        float getWidth() { return b.x - a.x; }
        float getHeight() { return b.y - a.y; }
    };
}

#endif

