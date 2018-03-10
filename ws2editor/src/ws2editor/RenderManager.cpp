#include "ws2editor/RenderManager.hpp"
#include "ws2editor/glplatform.hpp"
#include "ws2editor/GLManager.hpp"
#include "ws2editor/rendering/MeshRenderCommand.hpp"

namespace WS2Editor {
    void RenderManager::loadMesh(const MeshSegment *mesh) {
        using namespace WS2Common::Model;

        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->getVertices().size() * sizeof(Vertex), &mesh->getVertices()[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndices().size() * sizeof(unsigned int), &mesh->getIndices()[0], GL_STATIC_DRAW);

        //Vertex positions
        glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_POSITION);
        glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

        //Vertex normals
        glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_NORMAL);
        glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

        //Vertex texture coordinates
        glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_TEX_COORD);
        glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));

        glBindVertexArray(0);

        CachedGlMesh *cachedMesh = new CachedGlMesh;
        cachedMesh->setVao(vao);
        cachedMesh->setVbo(vbo);
        cachedMesh->setEbo(ebo);
        cachedMesh->setTriCount(mesh->getIndices().size());
        cachedMesh->getTextures().append(mesh->getTextures());

        meshCache[mesh] = cachedMesh;
    }

    void RenderManager::enqueueRenderMesh(const MeshSegment *mesh) {
        //First check if the mesh has been cached already
        //If it hasen't, we need to load it first
        if (!meshCache.contains(mesh)) loadMesh(mesh);

        //TODO: Update cache access time thingy
        renderFifo.enqueue(new MeshRenderCommand(meshCache[mesh]));
    }

    void RenderManager::renderQueue() {
        using namespace WS2Editor::Rendering;

        foreach(IRenderCommand *cmd, renderFifo) cmd->draw();
        qDeleteAll(renderFifo);
        renderFifo.clear();
    }
}

