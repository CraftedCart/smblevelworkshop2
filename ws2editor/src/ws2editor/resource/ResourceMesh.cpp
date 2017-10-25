#include "ws2editor/resource/ResourceMesh.hpp"
#include "ws2editor/glplatform.hpp"
#include "ws2editor/GLManager.hpp"
#include "ws2common/model/Vertex.hpp"
#include <QtAlgorithms>

namespace WS2Editor {
    namespace Resource {
        ResourceMesh::ResourceMesh() {}

        ResourceMesh::~ResourceMesh() {
            unload();
            qDeleteAll(meshSegments);
        }

        void ResourceMesh::load() {
            for (Model::EditorMeshSegment *segment : meshSegments) {
                segment->load();
            }

            loaded = true;
        }

        void ResourceMesh::unload() {
            loaded = false;

            for (Model::EditorMeshSegment *segment : meshSegments) {
                segment->unload();
            }
        }

        const QVector<Model::EditorMeshSegment*>& ResourceMesh::getMeshSegments() const {
            return meshSegments;
        }

        void ResourceMesh::addMeshSegment(Model::EditorMeshSegment *segment) {
            meshSegments.append(segment);
        }
    }
}

