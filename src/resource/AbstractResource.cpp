#include "resource/AbstractResource.hpp"

namespace WS2 {
    namespace Resource {
        AbstractResoruce::~AbstractResoruce() {
            if (isLoaded()) unload();
        }

        void AbstractResoruce::setFilePath(QString &filePath) {
            this->filePath = filePath;
        }

        const QString& AbstractResoruce::getFilePath() const {
            return filePath;
        }

        void AbstractResoruce::load() {}

        void AbstractResoruce::unload() {}

        bool AbstractResoruce::isLoaded() {
            return loaded;
        }
    }
}

