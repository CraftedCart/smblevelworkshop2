#include "ws2editor/resource/ResourceEditorMesh.hpp"
#include "ws2editor/model/EditorMeshSegment.hpp"

namespace WS2Editor {
    namespace Resource {
        void ResourceEditorMesh::load() {
            for (WS2Common::Model::MeshSegment *segment : meshSegments) {
                static_cast<Model::EditorMeshSegment*>(segment)->load();
            }

            loaded = true;
        }

        void ResourceEditorMesh::unload() {
            loaded = false;

            for (WS2Common::Model::MeshSegment *segment : meshSegments) {
                static_cast<Model::EditorMeshSegment*>(segment)->unload();
            }
        }
    }
}
