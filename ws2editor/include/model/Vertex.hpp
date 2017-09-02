/**
 * @file
 * @brief Header for the WS2::Model::Vertex struct
 */

#ifndef SMBLEVELWORKSHOP2_MODEL_VERTEX_HPP
#define SMBLEVELWORKSHOP2_MODEL_VERTEX_HPP

#include "glplatform.hpp"

namespace WS2 {
    namespace Model {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
        };
    }
}

#endif

