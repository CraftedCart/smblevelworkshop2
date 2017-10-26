#include "ws2editor/resource/ResourceEditorMesh.hpp"
#include "ws2editor/model/EditorMeshSegment.hpp"
#include "ws2common/WS2Common.hpp"

namespace WS2Editor {
    namespace Resource {
        ResourceEditorMesh::ResourceEditorMesh() {}

        ResourceEditorMesh::ResourceEditorMesh(const ResourceEditorMesh &origin) {
            QVector<WS2Common::Model::MeshSegment*> newMeshSegments(origin.getMeshSegments().size());

            //Check for non-ResourceEditorMeshes, and convert them over
            int i = 0;
            foreach(WS2Common::Model::MeshSegment* segment, origin.getMeshSegments()) {
                if (WS2Common::instanceOf<Model::EditorMeshSegment>(segment)) {
                    //Copy it over
                    newMeshSegments[i] = new Model::EditorMeshSegment(*static_cast<Model::EditorMeshSegment*>(segment));
                } else {
                    //Convert it
                    newMeshSegments[i] = new Model::EditorMeshSegment(*segment);
                }
            }
        }

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
