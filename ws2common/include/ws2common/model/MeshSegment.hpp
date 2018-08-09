/**
 * @file
 * @brief Header for the MeshSegment class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_MESHSEGMENT_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_MODEL_MESHSEGMENT_HPP

#include "ws2common_export.h"
#include "ws2common/model/Vertex.hpp"
#include "ws2common/resource/ResourceTexture.hpp"
#include "ws2common/AABB.hpp"
#include <QVector>

namespace WS2Common {
    namespace Model {
        /**
         * @brief Contains the data for one material in a 3D model
         *
         * @note Destroying this will not delete the textures
         */
        class WS2COMMON_EXPORT MeshSegment {
            protected:
                QVector<Vertex> vertices;
                QVector<unsigned int> indices;
                QVector<Resource::ResourceTexture*> textures;

                /**
                 * @brief The bounding box for this MeshSegment
                 */
                AABB3 aabb;

            public:
                /**
                 * @brief Create a new Mesh object with the arguments given and calculates its bounding box (AABB)
                 *
                 * @param vertices
                 * @param indices
                 * @param textures
                 */
                MeshSegment(
                        QVector<Vertex> vertices,
                        QVector<unsigned int> indices,
                        QVector<Resource::ResourceTexture*> textures);

                virtual ~MeshSegment();

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
                QVector<Resource::ResourceTexture*>& getTextures();

                /**
                 * @return A const reference to the textures vector
                 */
                const QVector<Resource::ResourceTexture*>& getTextures() const;

                /**
                 * @brief Gets the bounding box for this mesh segment
                 *
                 * @return This mesh segment's axis aligned bounding box
                 */
                const AABB3& getAabb() const;
        };
    }
}

#endif

