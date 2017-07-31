/**
 * @file
 * @brief Header for the WS2::Model::Mesh class
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCE_RESOURCEMESH_HPP
#define SMBLEVELWORKSHOP2_RESOURCE_RESOURCEMESH_HPP

#include "model/MeshSegment.hpp"
#include "resource/AbstractResource.hpp"
#include <QVector>

namespace WS2 {
    namespace Resource {
        /**
         * @todo Load and unload functions
         */
        class ResourceMesh : public AbstractResource {
            Q_OBJECT

            protected:
                /**
                 * @brief A vector containing pointers to MeshSegments owned by this ResourceMesh
                 */
                QVector<Model::MeshSegment*> meshSegments;

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
                ~ResourceMesh();

                /**
                 * @brief Generates GL buffers for the mesh
                 */
                void load() override;

                /**
                 * @brief Deletes the GL buffers for the mesh
                 */
                void unload() override;

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
        };

    }
}

#endif

