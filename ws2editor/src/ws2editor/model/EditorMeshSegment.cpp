#include "ws2editor/model/EditorMeshSegment.hpp"
#include "ws2editor/GLManager.hpp"

namespace WS2Editor {
    namespace Model {
        EditorMeshSegment::EditorMeshSegment(
                QVector<WS2Common::Model::Vertex> vertices,
                QVector<unsigned int> indices,
                QVector<Resource::ResourceEditorTexture*> textures) :
            WS2Common::Model::MeshSegment(vertices, indices, vectorToBaseTexture(textures)) {}

        void EditorMeshSegment::load() {
            generateGlBuffers();
        }

        void EditorMeshSegment::unload() {
            GLuint buffers[] = {vao, vbo, ebo};
            glDeleteBuffers(3, buffers);
        }

        void EditorMeshSegment::generateGlBuffers() {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(WS2Common::Model::Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            //Vertex positions
            glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_POSITION);
            glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(WS2Common::Model::Vertex), (void*) offsetof(WS2Common::Model::Vertex, position));

            //Vertex normals
            glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_NORMAL);
            glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(WS2Common::Model::Vertex), (void*) offsetof(WS2Common::Model::Vertex, normal));

            //Vertex texture coordinates
            glEnableVertexAttribArray(GLManager::VertexAttribs::VERTEX_TEX_COORD);
            glVertexAttribPointer(GLManager::VertexAttribs::VERTEX_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(WS2Common::Model::Vertex), (void*) offsetof(WS2Common::Model::Vertex, texCoord));

            glBindVertexArray(0);
        }

        const GLuint& EditorMeshSegment::getVao() const {
            return vao;
        }

        QVector<WS2Common::Resource::ResourceTexture*> EditorMeshSegment::vectorToBaseTexture(QVector<Resource::ResourceEditorTexture*> vec) {
            QVector<WS2Common::Resource::ResourceTexture*> out(vec.size());

            int i = 0;
            foreach(Resource::ResourceEditorTexture* tex, vec) {
                out[i] = tex;
                i++;
            }

            return out;
        }
    }
}

