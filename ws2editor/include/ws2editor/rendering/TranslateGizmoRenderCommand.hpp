/**
 * @file
 * @brief Header for the TranslateGizmoRenderCommand class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_TRANSLATE_GIZMORENDERCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERING_TRANSLATE_GIZMORENDERCOMMAND_HPP

#include "ws2editor/CachedGlMesh.hpp"
#include "ws2editor/rendering/IRenderCommand.hpp"
#include "ws2editor/RenderManager.hpp"

namespace WS2Editor {
    namespace Rendering {
        class TranslateGizmoRenderCommand : public IRenderCommand {
            protected:
                RenderManager *renderManager;
                glm::mat4 transform;
                glm::mat4 viewMatrix;
                glm::mat4 projMatrix;
                glm::vec3 cameraPos;
                bool highlightGizmoX;
                bool highlightGizmoY;
                bool highlightGizmoZ;

            public:
                TranslateGizmoRenderCommand(
                        RenderManager *renderManager,
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

#endif

