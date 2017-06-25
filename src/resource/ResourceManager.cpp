#include "resource/ResourceManager.hpp"
#include <algorithm>

namespace WS2 {
    namespace Resource {
        namespace ResourceManager {
            //Declare extern variables
            QVector<AbstractResoruce*> resources;

            void addResource(AbstractResoruce *res) {
                resources.append(res);
            }
        }
    }
}

