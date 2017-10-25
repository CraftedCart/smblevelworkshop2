/**
 * @file
 * @brief Header for the Vertex struct
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_MODEL_VERTEX_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_MODEL_VERTEX_HPP

#include <glm/glm.hpp>

namespace WS2Common {
    namespace Model {

        /**
         * @brief Stores data for a single vertex
         */
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
        };
    }
}

#endif

