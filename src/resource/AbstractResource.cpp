#include "resource/AbstractResource.hpp"

namespace WS2 {
    namespace Resource {
        AbstractResoruce::~AbstractResoruce() {
            if (isLoaded()) unload();
        }

        void AbstractResoruce::setFilePath(const QString &filePath) {
            filePaths.clear();
            filePaths.append(filePath);
        }

        void AbstractResoruce::addFilePath(const QString &filePath) {
            filePaths.append(filePath);
        }

        const QString* AbstractResoruce::getFirstFilePath() const {
            if (filePaths.size() > 0) {
                return &filePaths.at(0);
            } else {
                return nullptr;
            }
        }

        const QVector<QString>& AbstractResoruce::getFilePaths() const {
            return filePaths;
        }

        void AbstractResoruce::load() {}

        void AbstractResoruce::unload() {}

        bool AbstractResoruce::isLoaded() {
            return loaded;
        }
    }
}

