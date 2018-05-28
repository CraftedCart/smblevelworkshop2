#include "gizmosplugin/rendering/TranslateGizmoRenderCommand.hpp"
#include "gizmosplugin/RenderGlobals.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        namespace Rendering {
            using namespace WS2Editor;
            using namespace WS2Common::Resource;
            using namespace WS2Common::Model;

            TranslateGizmoRenderCommand::TranslateGizmoRenderCommand(
                    RenderManager *renderManager,
                    glm::mat4 transform,
                    glm::mat4 viewMatrix,
                    glm::mat4 projMatrix,
                    glm::vec3 cameraPos,
                    bool highlightGizmoX,
                    bool highlightGizmoY,
                    bool highlightGizmoZ
                    ) :
            renderManager(renderManager),
            transform(transform),
            viewMatrix(viewMatrix),
            projMatrix(projMatrix),
            cameraPos(cameraPos),
            highlightGizmoX(highlightGizmoX),
            highlightGizmoY(highlightGizmoY),
            highlightGizmoZ(highlightGizmoZ) {}

            void TranslateGizmoRenderCommand::draw() {
                //Scale the object so that it's the same scale regardless of distance from the camera
                glm::vec3 transformPos = glm::vec3(transform[3]);
                float dist = (projMatrix * viewMatrix * glm::vec4(transformPos, 1.0f)).w;
                glm::mat4 scale = glm::scale(glm::vec3(dist * 0.15f));

                glUseProgram(renderManager->unlitProgID);

                glDisable(GL_DEPTH_TEST);

                CachedGlMesh *mesh = renderManager->getCachedGlMesh(RenderGlobals::lineMeshSegment);

                glm::vec4 rTint(1.0f, 0.0f, 0.0f, 1.0f);
                glm::vec4 gTint(0.0f, 1.0f, 0.0f, 1.0f);
                glm::vec4 bTint(0.0f, 0.0f, 1.0f, 1.0f);

                //Highlight selected gizmos in yellow
                if (highlightGizmoX) rTint = glm::vec4(1.0f, 0.92f, 0.23f, 1.0f);
                if (highlightGizmoY) gTint = glm::vec4(1.0f, 0.92f, 0.23f, 1.0f);
                if (highlightGizmoZ) bTint = glm::vec4(1.0f, 0.92f, 0.23f, 1.0f);

                glUniformMatrix4fv(renderManager->unlitShaderViewID, 1, GL_FALSE, &viewMatrix[0][0]);
                glUniformMatrix4fv(renderManager->unlitShaderProjID, 1, GL_FALSE, &projMatrix[0][0]);

                //Don't draw the gizmo to the camera normal texture (position = 1)
                //TODO: Store positions in code elsewhere rather than hardcoding 1 here
                glColorMaski(1, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

                glBindVertexArray(mesh->getVao());

                //Draw the segment
                glm::mat4 yModel = transform * scale;
                glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &yModel[0][0]);
                glUniform4fv(renderManager->unlitShaderTintID, 1, &gTint[0]);
                glDrawElements(GL_LINES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);

                glm::mat4 xModel = transform * scale * glm::rotate(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, -1.0f));
                glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &xModel[0][0]);
                glUniform4fv(renderManager->unlitShaderTintID, 1, &rTint[0]);
                glDrawElements(GL_LINES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);

                glm::mat4 zModel = transform * scale * glm::rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
                glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &zModel[0][0]);
                glUniform4fv(renderManager->unlitShaderTintID, 1, &bTint[0]);
                glDrawElements(GL_LINES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);

                //Draw the cones
                for (ResourceMesh *coneResourceMesh : RenderGlobals::gizmoConeMesh) {
                    for (MeshSegment *segment : coneResourceMesh->getMeshSegments()) {
                        CachedGlMesh *coneMesh = renderManager->getCachedGlMesh(segment);
                        glBindVertexArray(coneMesh->getVao());

                        glm::mat4 yConeTranslateMat = glm::translate(glm::vec3(0.0f, 0.15f, 0.0f) * dist);
                        glm::mat4 yConeModel = transform * yConeTranslateMat * scale;
                        glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &yConeModel[0][0]);
                        glUniform4fv(renderManager->unlitShaderTintID, 1, &gTint[0]);
                        glDrawElements(GL_TRIANGLES, coneMesh->getTriCount(), GL_UNSIGNED_INT, 0);

                        glm::mat4 xConeTranslateMat = glm::translate(glm::vec3(0.15f, 0.0f, 0.0f) * dist);
                        glm::mat4 xConeModel = transform * xConeTranslateMat * scale * glm::rotate(glm::half_pi<float>(), glm::vec3(0.0f, 0.0f, -1.0f));
                        glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &xConeModel[0][0]);
                        glUniform4fv(renderManager->unlitShaderTintID, 1, &rTint[0]);
                        glDrawElements(GL_TRIANGLES, coneMesh->getTriCount(), GL_UNSIGNED_INT, 0);

                        glm::mat4 zConeTranslateMat = glm::translate(glm::vec3(0.0f, 0.0f, 0.15f) * dist);
                        glm::mat4 zConeModel = transform * zConeTranslateMat * scale * glm::rotate(glm::half_pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
                        glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &xConeModel[0][0]);
                        glUniformMatrix4fv(renderManager->unlitShaderModelID, 1, GL_FALSE, &zConeModel[0][0]);
                        glUniform4fv(renderManager->unlitShaderTintID, 1, &bTint[0]);
                        glDrawElements(GL_TRIANGLES, coneMesh->getTriCount(), GL_UNSIGNED_INT, 0);
                    }
                }

                //Draw to the camera normal tex again
                glColorMaski(1, GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

                glBindVertexArray(0);
                glEnable(GL_DEPTH_TEST);
            }
        }
    }
}

