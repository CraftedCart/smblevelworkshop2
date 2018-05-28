#include "ws2editor/rendering/MeshRenderCommand.hpp"

namespace WS2Editor {
    namespace Rendering {
        MeshRenderCommand::MeshRenderCommand(
                CachedGlMesh *mesh,
                RenderManager *renderManager,
                glm::mat4 transform,
                glm::vec4 tint,
                bool renderCameraNormals
                ) :
            mesh(mesh),
            renderManager(renderManager),
            transform(transform),
            tint(tint),
            renderCameraNormals(renderCameraNormals) {}

        void MeshRenderCommand::setMesh(CachedGlMesh *mesh) {
            this->mesh = mesh;
        }

        CachedGlMesh* MeshRenderCommand::getMesh() {
            return mesh;
        }

        void MeshRenderCommand::setTransform(glm::mat4 transform) {
            this->transform = transform;
        }

        glm::mat4 MeshRenderCommand::getTransform() {
            return transform;
        }

        void MeshRenderCommand::setRenderCameraNormals(bool renderCameraNormals) {
            this->renderCameraNormals = renderCameraNormals;
        }

        bool MeshRenderCommand::getRenderCameraNormals() {
            return renderCameraNormals;
        }

        void MeshRenderCommand::setTint(glm::vec4 tint) {
            this->tint = tint;
        }

        glm::vec4 MeshRenderCommand::getTint() {
            return tint;
        }

        void MeshRenderCommand::draw() {
            mesh->updateAccessTimer();

            //Set up textures
            glActiveTexture(GL_TEXTURE0);
            float texInfluence; //1.0f when textured, 0.0f otherwise

            if (mesh->getTextures().size() > 0) {
                CachedGlTexture *tex = renderManager->getTextureForResourceTexture(mesh->getTextures().at(0));
                tex->updateAccessTimer();
                glBindTexture(GL_TEXTURE_2D, tex->getTextureId());

                texInfluence = 1.0f;
            } else {
                glBindTexture(GL_TEXTURE_2D, 0);

                texInfluence = 0.0f;
            }

            glUniformMatrix4fv(renderManager->shaderModelID, 1, GL_FALSE, &transform[0][0]);
            glUniform1i(renderManager->shaderRenderCameraNormals, renderCameraNormals);
            glUniform1f(renderManager->shaderTexInfluenceID, texInfluence);
            glUniform4fv(renderManager->shaderTintID, 1, &tint[0]);

            //Draw the segment
            glBindVertexArray(mesh->getVao());
            glDrawElements(GL_TRIANGLES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}

