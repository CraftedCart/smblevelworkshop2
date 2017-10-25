#include "ws2editor/resource/ResourceEditorTexture.hpp"
#include "ws2editor/GLManager.hpp"
#include <QDebug>

namespace WS2Editor {
    namespace Resource {
        void ResourceEditorTexture::load() {
            WS2Common::Resource::ResourceTexture::load(); //Call super function

            qDebug() << "Loading GL texture:" << id;
            setGlTexture(GLManager::loadTexture(texture));
        }

        void ResourceEditorTexture::unload() {
            WS2Common::Resource::ResourceTexture::unload(); //Call super function

            GLuint texID = glTexture->textureId();
            glDeleteTextures(1, &texID);

            delete glTexture;
        }

        QOpenGLTexture* ResourceEditorTexture::getGlTexture() {
            return glTexture;
        }

        void ResourceEditorTexture::setGlTexture(QOpenGLTexture *tex) {
            glTexture = tex;
            loaded = tex != nullptr;
        }
    }
}

