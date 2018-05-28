/**
 * @file
 * @brief Header for the WS2Editor::CachedGlMesh class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_CACHEDGLMESH_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_CACHEDGLMESH_HPP

#include "ws2editor_export.h"
#include "ws2editor/glplatform.hpp"
#include "ws2editor/CachedGlTexture.hpp"
#include "ws2common/resource/ResourceTexture.hpp"
#include <QVector>

namespace WS2Editor {
    /**
     * @note The destructor will not free up the buffers on the GPU - it is your job to ensure that the correct GL context is bound and delete them yourself
     */
    class WS2EDITOR_EXPORT CachedGlMesh {
        protected:
            QElapsedTimer lastAccessTimer;

            GLuint vao;
            GLuint vbo;
            GLuint ebo;
            GLuint triCount;
            QVector<const WS2Common::Resource::ResourceTexture*> textures;

        public:
            void updateAccessTimer();
            QElapsedTimer& getLastAccessTimer();

            void setVao(GLuint vao);
            GLuint getVao() const;
            void setVbo(GLuint vbo);
            GLuint getVbo() const;
            void setEbo(GLuint ebo);
            GLuint getEbo() const;
            void setTriCount(GLuint triCount);
            GLuint getTriCount() const;
            QVector<const WS2Common::Resource::ResourceTexture*>& getTextures();
    };
}

#endif

