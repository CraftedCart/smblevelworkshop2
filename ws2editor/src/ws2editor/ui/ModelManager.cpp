#include "ws2editor/ui/ModelManager.hpp"

namespace WS2Editor {
    namespace UI {
        namespace ModelManager {
            //Define the extern members
            ModelResources *modelResources = new ModelResources(nullptr);
            ModelOutliner *modelOutliner = new ModelOutliner(nullptr);

            void destruct() {
                delete modelResources;
                delete modelOutliner;
            }
        }
    }
}

