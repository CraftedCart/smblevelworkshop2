/**
 * @file
 * @brief Header for the AABB (Axis-Aligned Bounding Box) structs
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_AABB_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_AABB_HPP

#include <glm/glm.hpp>
#include <algorithm>

namespace WS2Common {
    struct AABB2 {
        glm::vec2 a;
        glm::vec2 b;

        AABB2() {
            a = glm::vec2(0.0f);
            b = glm::vec2(0.0f);
        }

        AABB2(glm::vec2 a, glm::vec2 b) {
            this->a = a;
            this->b = b;
        }

        float getX() { return a.x; }
        float getY() { return a.y; }
        float getWidth() { return b.x - a.x; }
        float getHeight() { return b.y - a.y; }
    };

    struct AABB3 {
        glm::vec3 a;
        glm::vec3 b;

        AABB3() {
            a = glm::vec3(0.0f);
            b = glm::vec3(0.0f);
        }

        AABB3(glm::vec3 a, glm::vec3 b) {
            this->a = a;
            this->b = b;
        }

        float getMinX() { return a.x; }
        float getMinY() { return a.y; }
        float getMinZ() { return a.z; }
        float getMaxX() { return b.x; }
        float getMaxY() { return b.y; }
        float getMaxZ() { return b.z; }
        float getSizeX() { return b.x - a.x; }
        float getSizeY() { return b.y - a.y; }
        float getSizeZ() { return b.z - a.z; }

        /**
         * @brief Merges this AABB with other, taking the lowest min points and largest max points
         *
         * @param other The other AABB to merge into this
         */
        void mergeWith(const AABB3 &other) {
            a.x = std::min(a.x, other.a.x);
            a.y = std::min(a.y, other.a.y);
            a.z = std::min(a.z, other.a.z);

            b.x = std::max(b.x, other.b.x);
            b.y = std::max(b.y, other.b.y);
            b.z = std::max(b.z, other.b.z);
        }

        /**
         * @brief Merges this AABB with other, taking the lowest min points and largest max points
         *
         * @param other The other vec3 to merge into this
         */
        void mergeWith(const glm::vec3 &other) {
            a.x = std::min(a.x, other.x);
            a.y = std::min(a.y, other.y);
            a.z = std::min(a.z, other.z);

            b.x = std::max(b.x, other.x);
            b.y = std::max(b.y, other.y);
            b.z = std::max(b.z, other.z);
        }
    };
}

#endif

