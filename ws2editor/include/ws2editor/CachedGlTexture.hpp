/**
 * @file
 * @brief Header for the WS2Editor::CachedGlTexture class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_CACHEDGLTEXTURE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_CACHEDGLTEXTURE_HPP

#include <QOpenGLTexture>

namespace WS2Editor {

    /**
     * @note The destructor will not delete the texture - it is your job to ensure that the correct GL context is bound and delete it yourself
     */
    class CachedGlTexture {
        protected:
            QOpenGLTexture *texture;

        public:
            void setTexture(QOpenGLTexture *texture);
            QOpenGLTexture* getTexture();
    };
}

#endif

