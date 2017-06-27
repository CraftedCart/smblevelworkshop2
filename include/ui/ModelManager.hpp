/**
 * @file
 * @brief Header for the WS2::UI::ModelManager namespace
 */

#ifndef SMBLEVELWORKSHOP2_UI_MODELMANAGER_HPP
#define SMBLEVELWORKSHOP2_UI_MODELMANAGER_HPP

#include "ui/ModelResources.hpp"

namespace WS2 {
    namespace UI {
        namespace ModelManager {
            extern ModelResources *modelResources;

            /**
             * @brief Deletes pointers belonging to WS2::UI::ModelManager
             */
            void destruct();
        }
    }
}

#endif

