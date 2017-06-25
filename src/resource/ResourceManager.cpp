#include "resource/ResourceManager.hpp"
#include <algorithm>

namespace WS2 {
    namespace Resource {
        namespace ResourceManager {
            void addResource(AbstractResoruce *res) {
                resources.append(res);
            }

            template <class T> T* getResourceFromFilePath(QString filePath) {
                auto res = std::find_if(
                        resources.begin(),
                        resources.end(),
                        [&filePath](const AbstractResoruce *object) { return object->getFilePath() == filePath; }
                        );

                if (res != resources.end()) {
                    //Resource found
                    return static_cast<T>(res);
                } else {
                    //Resource not found
                    return nullptr;
                }
            }
        }
    }
}

