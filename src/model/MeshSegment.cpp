#include "model/MeshSegment.hpp"
#include "GLManager.hpp"

namespace WS2 {
    namespace Model {
        MeshSegment::MeshSegment(QVector<Model::Vertex> vertices, QVector<unsigned int> indices, QVector<Resource::ResourceTexture*> textures) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
        }

        void MeshSegment::load() {
            generateGlBuffers();
        }

        void MeshSegment::unload() {
            GLuint buffers[] = {vao, vbo, ebo};
            glDeleteBuffers(3, buffers);
        }

        void MeshSegment::generateGlBuffers() {
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

        const QVector<Model::Vertex>& MeshSegment::getVertices() const {
            return vertices;
        }

        const QVector<unsigned int>& MeshSegment::getIndices() const {
            return indices;
        }

        const QVector<Resource::ResourceTexture*>& MeshSegment::getTextures() const {
            return textures;
        }

        const GLuint& MeshSegment::getVao() const {
            return vao;
        }
    }
}

