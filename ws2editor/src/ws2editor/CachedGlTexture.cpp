#include "ws2editor/CachedGlTexture.hpp"

namespace WS2Editor {
    void CachedGlTexture::updateAccessTimer() {
        lastAccessTimer.start();
    }

    QElapsedTimer& CachedGlTexture::getLastAccessTimer() {
        return lastAccessTimer;
    }

    void CachedGlTexture::setTextureId(GLuint textureId) {
        this->textureId = textureId;
    }

    GLuint CachedGlTexture::getTextureId() {
        return textureId;
    }
}

