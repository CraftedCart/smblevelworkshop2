#include "resource/ResourceMesh.hpp"
#include "glplatform.hpp"
#include "GLManager.hpp"
#include "model/Vertex.hpp"
#include <QDebug>

namespace WS2 {
    namespace Resource {
        ResourceMesh::ResourceMesh() {}

        ResourceMesh::ResourceMesh(QVector<Model::Vertex> vertices, QVector<unsigned int> indices, QVector<Resource::ResourceTexture*> textures) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
        }

        void ResourceMesh::load() {
            generateGlBuffers();

            loaded = true;
        }

        void ResourceMesh::unload() {
            loaded = false;

            GLuint buffers[] = {vao, vbo, ebo};
            glDeleteBuffers(3, buffers);
        }

        void ResourceMesh::generateGlBuffers() {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Model::Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            //Vertex positions
            glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_POSITION);
            glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*) offsetof(Model::Vertex, position));

            //Vertex normals
            glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_NORMAL);
            glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*) offsetof(Model::Vertex, normal));

            //Vertex texture coordinates
            glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_TEX_COORD);
            glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Model::Vertex), (void*) offsetof(Model::Vertex, texCoord));

            glBindVertexArray(0);
        }

        const QVector<Model::Vertex>& ResourceMesh::getVertices() const {
            return vertices;
        }

        const QVector<unsigned int>& ResourceMesh::getIndices() const {
            return indices;
        }

        const QVector<Resource::ResourceTexture*>& ResourceMesh::getTextures() const {
            return textures;
        }

        const GLuint& ResourceMesh::getVao() const {
            return vao;
        }
    }
}

