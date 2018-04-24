#include "ws2editor/rendering/TranslateGizmoRenderCommand.hpp"
#include <glm/gtx/transform.hpp>

namespace WS2Editor {
    namespace Rendering {
        TranslateGizmoRenderCommand::TranslateGizmoRenderCommand(
                RenderManager *renderManager,
                glm::mat4 transform,
                glm::mat4 viewMatrix,
                glm::mat4 projMatrix,
                glm::vec3 cameraPos
                ) :
        renderManager(renderManager),
        transform(transform),
        viewMatrix(viewMatrix),
        projMatrix(projMatrix),
        cameraPos(cameraPos) {}

        void TranslateGizmoRenderCommand::draw() {
            //Scale the object so that it's the same scale regardless of distance from the camera
            glm::vec3 transformPos = glm::vec3(transform[3]);
            float dist = glm::distance(transformPos, cameraPos);
            transform = glm::scale(glm::vec3(dist * 0.2f)) * transform;

            glUseProgram(renderManager->unlitProgID);

            glDisable(GL_DEPTH_TEST);

            CachedGlMesh *mesh = renderManager->getCachedGlMesh(renderManager->lineMeshSegment);

            glm::vec4 rTint(1.0f, 0.0f, 0.0f, 1.0f);
            glm::vec4 gTint(0.0f, 1.0f, 0.0f, 1.0f);
            glm::vec4 bTint(0.0f, 0.0f, 1.0f, 1.0f);

            glUniformMatrix4fv(renderManager->unlitShaderViewID, 1, GL_FALSE, &viewMatrix[0][0]);
            glUniformMatrix4fv(renderManager->unlitShaderProjID, 1, GL_FALSE, &projMatrix[0][0]);

            //Don't draw the gizmo to the camera normal texture (position = 1)
            //TODO: Store positions in code elsewhere rather than hardcoding 1 here
            glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

            glBindVertexArray(mesh->getVao());

            //Draw the segment
            glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &transform[0][0]);
            glUniform4fv(renderManager->unlitShaderTintID, 1, &gTint[0]);
            glDrawElements(GL_LINES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);

            glm::mat4 xModel = glm::rotate(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, -1.0f)) * transform;
            glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &xModel[0][0]);
            glUniform4fv(renderManager->unlitShaderTintID, 1, &rTint[0]);
            glDrawElements(GL_LINES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);

            glm::mat4 zModel = glm::rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * transform;
            glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &zModel[0][0]);
            glUniform4fv(renderManager->unlitShaderTintID, 1, &bTint[0]);
            glDrawElements(GL_LINES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);

            //Don't draw
            glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

            glBindVertexArray(0);
            glEnable(GL_DEPTH_TEST);
        }
    }
}

