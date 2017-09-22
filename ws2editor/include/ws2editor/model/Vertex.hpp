/**
 * @file
 * @brief Header for the WS2Editor::Model::Vertex struct
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_VERTEX_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_VERTEX_HPP

#include "ws2editor/glplatform.hpp"

namespace WS2Editor {
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

