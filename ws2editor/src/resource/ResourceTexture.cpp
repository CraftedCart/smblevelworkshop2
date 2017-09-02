#include "resource/ResourceTexture.hpp"
#include "glplatform.hpp"
#include "GLManager.hpp"
#include <QDebug>

namespace WS2 {
    namespace Resource {
        void ResourceTexture::load() {
            const QString filePath = *getFirstFilePath();
            qDebug() << "Loading texture:" << filePath;

            QImage img(filePath);
            setTexture(GLManager::loadTexture(&img));
        }

        void ResourceTexture::unload() {
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

