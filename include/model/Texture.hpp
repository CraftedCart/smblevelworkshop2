/**
 * @file
 * @brief Header for the WS2::Model::Texture struct
 */

#ifndef SMBLEVELWORKSHOP2_MODEL_TEXTURE_HPP
#define SMBLEVELWORKSHOP2_MODEL_TEXTURE_HPP

#include "glplatform.hpp"
#include <QOpenGLTexture>

namespace WS2 {
    namespace Model {
        struct Texture {
            QOpenGLTexture *texture;
        };
    }
}

#endif

