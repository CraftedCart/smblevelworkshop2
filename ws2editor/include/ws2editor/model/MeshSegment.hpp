/**
 * @file
 * @brief Header for the MeshSegment class
 */

#ifndef SMBLEVELWORKSHOP2_MODEL_MESHSEGMENT_HPP
#define SMBLEVELWORKSHOP2_MODEL_MESHSEGMENT_HPP

#include "ws2editor/model/Vertex.hpp"
#include "ws2editor/resource/ResourceTexture.hpp"
#include "ws2editor/glplatform.hpp"
#include <QVector>

namespace WS2Editor {
    namespace Model {
        /**
         * @brief Contains the data for one material in a 3D model
         */
        class MeshSegment {
            protected:
                QVector<Model::Vertex> vertices;
                QVector<unsigned int> indices;
                QVector<Resource::ResourceTexture*> textures;

                GLuint vao;
                GLuint vbo;
                GLuint ebo;

                /**
                 * @brief Generates GL buffers
                 */
                void generateGlBuffers();

            public:
                /**
                 * @brief Create a new Mesh object with the arguments given
                 *
                 * @param vertices
                 * @param indices
                 * @param textures
                 */
                MeshSegment(QVector<Model::Vertex> vertices, QVector<unsigned int> indices, QVector<Resource::ResourceTexture*> textures);

                /**
                 * @brief Generates GL buffers for the mesh
                 */
                void load();

                /**
                 * @brief Deletes the GL buffers for the mesh
                 */
                void unload();

                /**
                 * @return A reference to the vertices vector
                 */
                const QVector<Model::Vertex>& getVertices() const;

                /**
                 * @return A reference to the indices vector
                 */
                const QVector<unsigned int>& getIndices() const;

                /**
                 * @return A reference to the textures vector
                 */
                const QVector<Resource::ResourceTexture*>& getTextures() const;

                /**
                 * @return The VAO ID of the mesh
                 */
                const GLuint& getVao() const;
        };
    }
}

#endif

