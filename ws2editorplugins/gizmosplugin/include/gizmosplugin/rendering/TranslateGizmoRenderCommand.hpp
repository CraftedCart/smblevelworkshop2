/**
 * @file
 * @brief Header for the TranslateGizmoRenderCommand class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_RENDERING_TRANSLATEGIZMORENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_RENDERING_TRANSLATEGIZMORENDERCOMMAND_HPP

#include "ws2editor/CachedGlMesh.hpp"
#include "ws2editor/rendering/IRenderCommand.hpp"
#include "ws2editor/RenderManager.hpp"

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        namespace Rendering {
            class TranslateGizmoRenderCommand : public WS2Editor::IRenderCommand {
                protected:
                    WS2Editor::RenderManager *renderManager;
                    glm::mat4 transform;
                    glm::mat4 viewMatrix;
                    glm::mat4 projMatrix;
                    glm::vec3 cameraPos;
                    bool highlightGizmoX;
                    bool highlightGizmoY;
                    bool highlightGizmoZ;

                public:
                    TranslateGizmoRenderCommand(
                            WS2Editor::RenderManager *renderManager,
                            glm::mat4 transform,
                            glm::mat4 viewMatrix,
                            glm::mat4 projMatrix,
                            glm::vec3 cameraPos,
                            bool highlightGizmoX,
                            bool highlightGizmoY,
                            bool highlightGizmoZ
                            );

                    virtual void draw() override;
            };
        }
    }
}

#endif

