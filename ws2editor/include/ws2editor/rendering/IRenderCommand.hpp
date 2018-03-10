/**
 * @file
 * @brief Header for the IRenderCommand interface
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_IRENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_IRENDERCOMMAND_HPP

namespace WS2Editor {
    namespace Rendering {
        class IRenderCommand {
            public:
                virtual ~IRenderCommand() {};
                virtual void draw() = 0;
        };
    }
}

#endif

