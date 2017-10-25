/**
 * @file
 * @brief Header for the WS2Editor::Model::Mesh class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCEMESH_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCEMESH_HPP

#include "ws2editor/model/EditorMeshSegment.hpp"
#include "ws2common/resource/AbstractResource.hpp"
#include <QVector>

namespace WS2Editor {
    namespace Resource {
        /**
         * @todo Load and unload functions
         */
        class ResourceMesh : public WS2Common::Resource::AbstractResource {
            Q_OBJECT

            protected:
                /**
                 * @brief A vector containing pointers to MeshSegments owned by this ResourceMesh
                 */
                QVector<Model::EditorMeshSegment*> meshSegments;

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
                const QVector<Model::EditorMeshSegment*>& getMeshSegments() const;

                /**
                 * @brief Adds a mesh segment that belongs to this ResourceMesh
                 *
                 * @param segment The segment to add
                 */
                void addMeshSegment(Model::EditorMeshSegment *segment);
        };

    }
}

#endif

