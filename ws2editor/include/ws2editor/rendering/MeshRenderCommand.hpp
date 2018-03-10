/**
 * @file
 * @brief Header for the MeshRenderCommand class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_MESHRENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_MESHRENDERCOMMAND_HPP

#include "ws2editor/CachedGlMesh.hpp"
#include "ws2editor/rendering/IRenderCommand.hpp"

namespace WS2Editor {
    namespace Rendering {
        /**
         * @note The destructor will not delete mesh - this is expected to be cached elsewhere, and manually deleted when no longer in use
         */
        class MeshRenderCommand : public IRenderCommand {
            protected:
                CachedGlMesh *mesh;

            public:
                MeshRenderCommand(CachedGlMesh *mesh);

                virtual void draw() override;
        };
    }
}

#endif

