#include "resource/ResourceManager.hpp"
#include "ui/ModelManager.hpp"
#include <algorithm>

namespace WS2 {
    namespace Resource {
        namespace ResourceManager {
            //Declare extern variables
            QVector<AbstractResoruce*> resources;

            QVector<AbstractResoruce*>& getResources() {
                return resources;
            }

            void addResource(AbstractResoruce *res) {
                resources.append(res);
                UI::ModelManager::modelResources->onResourceAdded();
            }
        }
    }
}

