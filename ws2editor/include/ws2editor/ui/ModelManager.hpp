/**
 * @file
 * @brief Header for the WS2Editor::UI::ModelManager namespace
 */

#ifndef SMBLEVELWORKSHOP2_UI_MODELMANAGER_HPP
#define SMBLEVELWORKSHOP2_UI_MODELMANAGER_HPP

#include "ws2editor/ui/ModelResources.hpp"
#include "ws2editor/ui/ModelOutliner.hpp"

namespace WS2Editor {
    namespace UI {
        namespace ModelManager {
            extern ModelResources *modelResources;
            extern ModelOutliner *modelOutliner;

            /**
             * @brief Deletes pointers belonging to WS2Editor::UI::ModelManager
             */
            void destruct();
        }
    }
}

#endif

