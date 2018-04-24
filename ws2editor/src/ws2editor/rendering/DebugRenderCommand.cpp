#include "ws2editor/rendering/DebugRenderCommand.hpp"

namespace WS2Editor {
    namespace Rendering {
        DebugRenderCommand::DebugRenderCommand(
                RenderManager *renderManager,
                glm::mat4 viewMatrix,
                glm::mat4 projMatrix,
                btDynamicsWorld *dynamicsWorld,
                PhysicsDebugDrawer *debugDrawer
                ) :
        renderManager(renderManager),
        viewMatrix(viewMatrix),
        projMatrix(projMatrix),
        dynamicsWorld(dynamicsWorld),
        debugDrawer(debugDrawer) {}

        void DebugRenderCommand::draw() {
            glUseProgram(renderManager->physicsDebugProgID);

            //Give the matrices to the bound shader
            glUniformMatrix4fv(renderManager->physicsDebugShaderViewID, 1, GL_TRUE, &viewMatrix[0][0]);
            glUniformMatrix4fv(renderManager->physicsDebugShaderProjID, 1, GL_TRUE, &projMatrix[0][0]);

            dynamicsWorld->debugDrawWorld();

            //Don't draw the gizmo to the camera normal texture (position = 1)
            //TODO: Store positions in code elsewhere rather than hardcoding 1 here
            glColorMaski(1, GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);

            debugDrawer->drawAll();

            glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        }
    }
}

