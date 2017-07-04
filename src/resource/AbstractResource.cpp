#include "resource/AbstractResource.hpp"

namespace WS2 {
    namespace Resource {
        AbstractResource::~AbstractResource() {
            if (isLoaded()) unload();
        }

        void AbstractResource::setId(const QString &id) {
            this->id = id;
        }

        QString& AbstractResource::getId() {
            return id;
        }

        void AbstractResource::setFilePath(const QString &filePath) {
            filePaths.clear();
            filePaths.append(filePath);
        }

        void AbstractResource::addFilePath(const QString &filePath) {
            filePaths.append(filePath);
        }

        const QString* AbstractResource::getFirstFilePath() const {
            if (filePaths.size() > 0) {
                return &filePaths.at(0);
            } else {
                return nullptr;
            }
        }

        const QVector<QString>& AbstractResource::getFilePaths() const {
            return filePaths;
        }

        void AbstractResource::load() {}

        void AbstractResource::unload() {}

        bool AbstractResource::isLoaded() {
            return loaded;
        }
    }
}

