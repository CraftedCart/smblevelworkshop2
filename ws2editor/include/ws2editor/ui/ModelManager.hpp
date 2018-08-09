/**
 * @file
 * @brief Header for the ModelManager namespace
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELMANAGER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELMANAGER_HPP

#include "ws2editor_export.h"
#include "ws2editor/ui/ModelResources.hpp"
#include "ws2editor/ui/ModelOutliner.hpp"

namespace WS2Editor {
    namespace UI {
        namespace ModelManager {
            WS2EDITOR_EXPORT extern ModelResources *modelResources;
            WS2EDITOR_EXPORT extern ModelOutliner *modelOutliner;

            /**
             * @brief Deletes pointers belonging to WS2Editor::UI::ModelManager
             */
            WS2EDITOR_EXPORT void destruct();
        }
    }
}

#endif

