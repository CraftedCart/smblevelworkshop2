#include "ws2editor/resource/ResourceEditorMesh.hpp"
#include "ws2editor/model/EditorMeshSegment.hpp"
#include "ws2common/WS2Common.hpp"

namespace WS2Editor {
    namespace Resource {
        ResourceEditorMesh::ResourceEditorMesh() {}

        ResourceEditorMesh::ResourceEditorMesh(const ResourceMesh &origin) {
            //Check for non-ResourceEditorMeshes, and convert them over
            foreach(WS2Common::Model::MeshSegment* segment, origin.getMeshSegments()) {
                if (dynamic_cast<Model::EditorMeshSegment*>(segment)) {
                    //Copy it over
                    meshSegments.append(new Model::EditorMeshSegment(*static_cast<Model::EditorMeshSegment*>(segment)));
                } else {
                    //Convert it
                    meshSegments.append(new Model::EditorMeshSegment(*segment));
                }
            }

            //Copy over the other guff
            id = origin.getId();
            filePaths = origin.getFilePaths();
        }

        void ResourceEditorMesh::load() {
            foreach(WS2Common::Model::MeshSegment *segment, meshSegments) {
                Model::EditorMeshSegment *editorSegment = static_cast<Model::EditorMeshSegment*>(segment);
                foreach (WS2Common::Resource::ResourceTexture *tex, editorSegment->getTextures()) {
                    tex->load();
                }

                editorSegment->load();
            }

            loaded = true;
        }

        void ResourceEditorMesh::unload() {
            loaded = false;

            for (WS2Common::Model::MeshSegment *segment : meshSegments) {
                static_cast<Model::EditorMeshSegment*>(segment)->unload();
                //TODO: Unload textures (But not ones in use by other meshes)
            }
        }
    }
}
