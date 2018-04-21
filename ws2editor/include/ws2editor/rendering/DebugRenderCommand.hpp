/**
 * @file
 * @brief Header for the DebugRenderCommand class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_DEBUGRENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_DEBUGRENDERCOMMAND_HPP

#include "ws2editor/CachedGlMesh.hpp"
#include "ws2editor/rendering/IRenderCommand.hpp"
#include "ws2editor/RenderManager.hpp"

namespace WS2Editor {
    namespace Rendering {
        /**
         * @note The destructor will not delete mesh - this is expected to be cached elsewhere, and manually deleted when no longer in use
         */
        class DebugRenderCommand : public IRenderCommand {
            protected:
                RenderManager *renderManager;
                glm::mat4 viewMatrix;
                glm::mat4 projMatrix;
                btDynamicsWorld *dynamicsWorld;
                PhysicsDebugDrawer *debugDrawer;

            public:
                DebugRenderCommand(
                        RenderManager *renderManager,
                        glm::mat4 viewMatrix,
                        glm::mat4 projMatrix,
                        btDynamicsWorld *dynamicsWorld,
                        PhysicsDebugDrawer *debugDrawer
                        );

                virtual void draw() override;
        };
    }
}

#endif

