/**
 * @file
 * @brief Header for the WS2::Model::Mesh class
 */

#ifndef SMBLEVELWORKSHOP2_MODEL_MESH_HPP
#define SMBLEVELWORKSHOP2_MODEL_MESH_HPP

#include "model/Vertex.hpp"
#include "model/Texture.hpp"
#include <QVector>

namespace WS2 {
    namespace Model {
        class Mesh {
            protected:
                QVector<Vertex> vertices;
                QVector<unsigned int> indices;
                QVector<Texture> textures;

                GLuint vao;
                GLuint vbo;
                GLuint ebo;

            public:
                /**
                 * @brief Create an empty mesh
                 *
                 * This exists only for the purpose of being able to use this in a QVector
                 */
                Mesh();

                /**
                 * @brief Create a new Mesh object with the arguments given
                 *
                 * @param vertices
                 * @param indices
                 * @param textures
                 *
                 * @note You should call WS2::Model::Mesh::generateGlBuffers() on the object before using it
                 */
                Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<Texture> textures);

                /**
                 * @brief Generates GL buffers
                 */
                void generateGlBuffers();

                /**
                 * @return The vertices vector
                 */
                const QVector<Vertex>& getVertices() const;

                /**
                 * @return The indices vector
                 */
                const QVector<unsigned int>& getIndices() const;

                /**
                 * @return The textures vector
                 */
                const QVector<Texture>& getTextures() const;

                /**
                 * @return The VAO ID of the mesh
                 */
                const GLuint& getVao() const;
        };

    }
}

#endif

