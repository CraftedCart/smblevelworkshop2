/**
 * @file
 * @brief Header for the WS2Editor::CachedGlMesh class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_CACHEDGLMESH_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_CACHEDGLMESH_HPP

#include "ws2editor/glplatform.hpp"
#include "ws2common/resource/ResourceTexture.hpp" //TODO: Temp
#include <QVector>

namespace WS2Editor {

    /**
     * @note The destructor will not free up the buffers on the GPU - it is your job to ensure that the correct GL context is bound and delete them yourself
     */
    class CachedGlMesh {
        protected:
            GLuint vao;
            GLuint vbo;
            GLuint ebo;
            GLuint triCount;
            QVector<WS2Common::Resource::ResourceTexture*> textures; //TODO: Temp

        public:
            void setVao(GLuint vao);
            GLuint getVao() const;
            void setVbo(GLuint vbo);
            GLuint getVbo() const;
            void setEbo(GLuint ebo);
            GLuint getEbo() const;
            void setTriCount(GLuint triCount);
            GLuint getTriCount() const;
            QVector<WS2Common::Resource::ResourceTexture*>& getTextures();
    };
}

#endif

