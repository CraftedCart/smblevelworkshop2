#include "ws2common/resource/ResourceTexture.hpp"

namespace WS2Common {
    namespace Resource {
        void ResourceTexture::load() {
            AbstractResource::load();

            const QString filePath = *getFirstFilePath();
            texture = new QImage(filePath);
        }

        void ResourceTexture::unload() {
            AbstractResource::unload();

            delete texture;
            texture = nullptr;
        }

        QImage* ResourceTexture::getTexture() {
            return texture;
        }

        const QImage* ResourceTexture::getTexture() const {
            return texture;
        }
    }
}

