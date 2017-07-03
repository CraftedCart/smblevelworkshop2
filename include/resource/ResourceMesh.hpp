/**
 * @file
 * @brief Header for the WS2::Model::Mesh class
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCE_RESOURCEMESH_HPP
#define SMBLEVELWORKSHOP2_RESOURCE_RESOURCEMESH_HPP

#include "model/Vertex.hpp"
#include "resource/ResourceTexture.hpp"
#include <QVector>

namespace WS2 {
    namespace Resource {
        /**
         * @todo Load and unload functions
         */
        class ResourceMesh : public AbstractResource {
            protected:
                QVector<Model::Vertex> vertices;
                QVector<unsigned int> indices;
                QVector<Resource::ResourceTexture*> textures;

                GLuint vao;
                GLuint vbo;
                GLuint ebo;

            public:
                /**
                 * @brief Create an empty mesh
                 *
                 * This exists only for the purpose of being able to use this in a QVector
                 */
                ResourceMesh();

                /**
                 * @brief Create a new Mesh object with the arguments given
                 *
                 * @param vertices
                 * @param indices
                 * @param textures
                 */
                ResourceMesh(QVector<Model::Vertex> vertices, QVector<unsigned int> indices, QVector<Resource::ResourceTexture*> textures);

                /**
                 * @brief Generates GL buffers for the mesh
                 */
                void load() override;

                /**
                 * @brief Deletes the GL buffers for the mesh
                 */
                void unload() override;

                /**
                 * @brief Generates GL buffers
                 */
                void generateGlBuffers();

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

