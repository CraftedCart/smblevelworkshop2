/**
 * @file
 * @brief Header for the WS2::UI::ModelManager namespace
 */

#ifndef SMBLEVELWORKSHOP2_UI_MODELMANAGER_HPP
#define SMBLEVELWORKSHOP2_UI_MODELMANAGER_HPP

#include "ui/ModelResources.hpp"
#include "ui/ModelOutliner.hpp"

namespace WS2 {
    namespace UI {
        namespace ModelManager {
            extern ModelResources *modelResources;
            extern ModelOutliner *modelOutliner;

            /**
             * @brief Deletes pointers belonging to WS2::UI::ModelManager
             */
            void destruct();
        }
    }
}

#endif

