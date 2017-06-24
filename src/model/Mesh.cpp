#include "model/Mesh.hpp"
#include "glplatform.hpp"
#include "GLManager.hpp"
#include "model/Vertex.hpp"
#include <QDebug>

namespace WS2 {
    namespace Model {
        Mesh::Mesh() {}

        Mesh::Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            generateGlBuffers(); //TODO: Don't call this in the constuctor
        }

        void Mesh::generateGlBuffers() {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
        }

        const QVector<Vertex>& Mesh::getVertices() const {
            return vertices;
        }

        const QVector<unsigned int>& Mesh::getIndices() const {
            return indices;
        }

        const QVector<Texture>& Mesh::getTextures() const {
            return textures;
        }

        const GLuint& Mesh::getVao() const {
            return vao;
        }
    }
}

