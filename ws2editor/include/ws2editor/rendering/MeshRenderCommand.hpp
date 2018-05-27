/**
 * @file
 * @brief Header for the MeshRenderCommand class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_MESHRENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_MESHRENDERCOMMAND_HPP

#include "ws2editor_export.h"
#include "ws2editor/CachedGlMesh.hpp"
#include "ws2editor/rendering/IRenderCommand.hpp"
#include "ws2editor/RenderManager.hpp"

namespace WS2Editor {
    namespace Rendering {
        /**
         * @note The destructor will not delete mesh - this is expected to be cached elsewhere, and manually deleted when no longer in use
         */
        class WS2EDITOR_EXPORT MeshRenderCommand : public IRenderCommand {
            protected:
                CachedGlMesh *mesh;
                RenderManager *renderManager;
                glm::mat4 transform;
                bool renderCameraNormals;
                glm::vec4 tint;

            public:
                MeshRenderCommand(
                        CachedGlMesh *mesh,
                        RenderManager *renderManager,
                        glm::mat4 transform = glm::mat4(1.0f),
                        glm::vec4 tint = glm::vec4(1.0f),
                        bool renderCameraNormals = false
                        );

                virtual void draw() override;
        };
    }
}

#endif

