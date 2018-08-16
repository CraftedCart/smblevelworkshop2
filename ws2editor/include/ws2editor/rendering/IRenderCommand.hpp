/**
 * @file
 * @brief Header for the IRenderCommand interface
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_IRENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_IRENDERCOMMAND_HPP

#include "ws2editor_export.h"
namespace WS2Editor {
    namespace Rendering {
        class WS2EDITOR_EXPORT IRenderCommand {
            public:
                virtual ~IRenderCommand() {};
                virtual void draw() = 0;
        };
    }
}

#endif

