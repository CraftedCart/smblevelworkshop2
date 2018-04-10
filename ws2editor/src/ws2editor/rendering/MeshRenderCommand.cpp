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
            glActiveTexture(GL_TEXTURE0);

            if (mesh->getTextures().size() > 0) {
                CachedGlTexture *tex = renderManager->getTextureForResourceTexture(mesh->getTextures().at(0));
                tex->updateAccessTimer();
                glBindTexture(GL_TEXTURE_2D, tex->getTextureId());
            } else {
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            glUniform1i(renderManager->shaderRenderCameraNormals, renderCameraNormals);

            //Draw the segment
            glBindVertexArray(mesh->getVao());
            glDrawElements(GL_TRIANGLES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}

