/**
 * @file
 * @brief Header for the ResourceMesh class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_RESOURCE_RESOURCEMESH_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_RESOURCE_RESOURCEMESH_HPP

#include "ws2common_export.h"
#include "ws2common/model/MeshSegment.hpp"
#include "ws2common/resource/AbstractResource.hpp"
#include <QVector>

namespace WS2Common {
    namespace Resource {
        /**
         * @todo Load and unload functions
         */
        class WS2COMMON_EXPORT ResourceMesh : public AbstractResource {
            Q_OBJECT

            protected:
                /**
                 * @brief A vector containing pointers to MeshSegments owned by this ResourceMesh
                 */
                QVector<Model::MeshSegment*> meshSegments;

                /**
                 * @brief This mesh's bounding box
                 */
                AABB3 aabb;

            public:
                /**
                 * @brief Create an empty mesh
                 *
                 * This exists only for the purpose of being able to use this in a QVector
                 */
                ResourceMesh();

                /**
                 * @brief Unloads and deletes all mesh segments
                 */
                virtual ~ResourceMesh();

                /**
                 * @brief Getter for meshSegments
                 *
                 * @return A reference to the meshSegments vector, with each segment containing info for one material
                 *         of the ResourceMesh
                 */
                const QVector<Model::MeshSegment*>& getMeshSegments() const;

                /**
                 * @brief Adds a mesh segment that belongs to this ResourceMesh
                 *
                 * @param segment The segment to add
                 */
                void addMeshSegment(Model::MeshSegment *segment);

                /**
                 * @brief Gets the bounding box for this mesh
                 *
                 * @return This mesh's axis aligned bounding box
                 */
                const AABB3& getAabb() const;
        };

    }
}

#endif

