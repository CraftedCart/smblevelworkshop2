#include "ws2editor/rendering/MeshRenderCommand.hpp"
#include "ws2editor/resource/ResourceEditorTexture.hpp"
#include "ws2editor/GLManager.hpp"

namespace WS2Editor {
    namespace Rendering {
        MeshRenderCommand::MeshRenderCommand(CachedGlMesh *mesh) :
            mesh(mesh) {}

        void MeshRenderCommand::draw() {
            //Set up textures
            for (int i = 0; i < mesh->getTextures().size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                //Assume that it's a ResourceEditorTexture
                static_cast<Resource::ResourceEditorTexture*>(mesh->getTextures().at(i))->getGlTexture()->bind();
            }
            static const int texIDs[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
            glUniform1iv(GLManager::shaderTexID, GLManager::MAX_SHADER_TEXTURES, texIDs);

            //Draw the segment
            glBindVertexArray(mesh->getVao());
            glDrawElements(GL_TRIANGLES, mesh->getTriCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}

