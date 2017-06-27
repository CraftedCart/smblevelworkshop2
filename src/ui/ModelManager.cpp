#include "ui/ModelManager.hpp"

namespace WS2 {
    namespace UI {
        namespace ModelManager {
            //Define the extern members
            ModelResources *modelResources = new ModelResources(nullptr);

            void destruct() {
                delete modelResources;
            }
        }
    }
}

