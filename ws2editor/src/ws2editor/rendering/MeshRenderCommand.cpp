#include "ws2editor/rendering/MeshRenderCommand.hpp"

namespace WS2Editor {
    namespace Rendering {
        MeshRenderCommand::MeshRenderCommand(CachedGlMesh *mesh, RenderManager *renderManager, bool renderCameraNormals) :
            mesh(mesh),
            renderManager(renderManager),
            renderCameraNormals(renderCameraNormals) {}

        void MeshRenderCommand::draw() {
            mesh->updateAccessTimer();

            //Set up textures
            for (int i = 0; i < renderManager->MAX_SHADER_TEXTURES; i++) {
                glActiveTexture(GL_TEXTURE0 + i);

                if (i < mesh->getTextures().size()) {
                    CachedGlTexture *tex = renderManager->getTextureForResourceTexture(mesh->getTextures().at(i));
                    tex->updateAccessTimer();
                    glBindTexture(GL_TEXTURE_2D, tex->getTextureId());
                } else {
                    glBindTexture(GL_TEXTURE_2D, 0);
                }
            }

            static const int texIDs[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
            glUniform1iv(renderManager->shaderTexID, renderManager->MAX_SHADER_TEXTURES, texIDs);
            glUniform1i(renderManager->shaderRenderCameraNormals, renderCameraNormals);

            //Draw the segment
            glBindVertexArray(mesh->getVao());
            glDrawElements(GL_TRIANGLES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}

