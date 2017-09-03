#include "resource/ResourceMesh.hpp"
#include "glplatform.hpp"
#include "GLManager.hpp"
#include "model/Vertex.hpp"
#include <QtAlgorithms>

namespace WS2Editor {
    namespace Resource {
        ResourceMesh::ResourceMesh() {}

        ResourceMesh::~ResourceMesh() {
            unload();
            qDeleteAll(meshSegments);
        }

        void ResourceMesh::load() {
            for (Model::MeshSegment *segment : meshSegments) {
                segment->load();
            }

            loaded = true;
        }

        void ResourceMesh::unload() {
            loaded = false;

            for (Model::MeshSegment *segment : meshSegments) {
                segment->unload();
            }
        }

        const QVector<Model::MeshSegment*>& ResourceMesh::getMeshSegments() const {
            return meshSegments;
        }

        void ResourceMesh::addMeshSegment(Model::MeshSegment *segment) {
            meshSegments.append(segment);
        }
    }
}

