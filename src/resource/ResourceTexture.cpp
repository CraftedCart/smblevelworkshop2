#include "resource/ResourceTexture.hpp"
#include "glplatform.hpp"
#include "GLManager.hpp"
#include <QDebug>

namespace WS2 {
    namespace Resource {
        void ResourceTexture::load() {
            qDebug() << "Loading texture:" << filePath;

            QImage img(filePath);
            setTexture(GLManager::loadTexture(&img));
        }

        void ResourceTexture::unload() {
            qDebug() << "Unloading texture:" << filePath;

            GLuint texID = texture->textureId();
            glDeleteTextures(1, &texID);

            delete texture;
        }

        QOpenGLTexture* ResourceTexture::getTexture() {
            return texture;
        }

        void ResourceTexture::setTexture(QOpenGLTexture *tex) {
            texture = tex;
            loaded = tex != nullptr;
        }
    }
}

