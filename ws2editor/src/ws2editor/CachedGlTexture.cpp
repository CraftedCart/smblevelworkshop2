#include "ws2editor/CachedGlTexture.hpp"

namespace WS2Editor {
    void CachedGlTexture::setTexture(QOpenGLTexture *texture) {
        this->texture = texture;
    }

    QOpenGLTexture* CachedGlTexture::getTexture() {
        return texture;
    }
}

