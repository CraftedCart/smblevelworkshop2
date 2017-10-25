/**
 * @file
 * @brief Header for the MeshSegment class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_MESHSEGMENT_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_MESHSEGMENT_HPP

#include "ws2common/model/Vertex.hpp"
#include "ws2common/resource/ResourceTexture.hpp"
#include <QVector>

namespace WS2Common {
    namespace Model {
        /**
         * @brief Contains the data for one material in a 3D model
         */
        class MeshSegment {
            protected:
                QVector<Vertex> vertices;
                QVector<unsigned int> indices;
                QVector<Resource::ResourceTexture*> textures;

            public:
                /**
                 * @brief Create a new Mesh object with the arguments given
                 *
                 * @param vertices
                 * @param indices
                 * @param textures
                 */
                MeshSegment(
                        QVector<Vertex> vertices,
                        QVector<unsigned int> indices,
                        QVector<Resource::ResourceTexture*> textures);

                /**
                 * @return A reference to the vertices vector
                 */
                const QVector<Vertex>& getVertices() const;

                /**
                 * @return A reference to the indices vector
                 */
                const QVector<unsigned int>& getIndices() const;

                /**
                 * @return A reference to the textures vector
                 */
                const QVector<Resource::ResourceTexture*>& getTextures() const;
        };
    }
}

#endif

