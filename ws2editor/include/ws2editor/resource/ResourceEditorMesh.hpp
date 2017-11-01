/**
 * @file
 * @brief Header for ResourceEditorMesh
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCEEDITORMESH_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCEEDITORMESH_HPP

#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2Editor {
    namespace Resource {
        /**
         * @brief Inherits from ResourceMesh, but provides load/unload functions to load/unload MeshSegment GL buffers
         */
        class ResourceEditorMesh : public WS2Common::Resource::ResourceMesh {
            public:
                ResourceEditorMesh();

                /**
                 * @brief Copy constructor, that also converts all MeshSegments to EditorMeshSegments
                 *
                 * @param origin The ResourceMesh to copy
                 */
                ResourceEditorMesh(const ResourceMesh &origin);

                /**
                 * @brief Generates GL buffers for the mesh
                 *
                 * Assumes all segments are EditorMeshSegments
                 */
                void load() override;

                /**
                 * @brief Deletes the GL buffers for the mesh
                 *
                 * Assumes all segments are EditorMeshSegments
                 */
                void unload() override;
        };
    }
}

#endif

