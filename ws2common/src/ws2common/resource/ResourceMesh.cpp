#include "ws2common/resource/ResourceMesh.hpp"
#include "ws2common/model/Vertex.hpp"
#include <QtAlgorithms>

namespace WS2Common {
    namespace Resource {
        ResourceMesh::ResourceMesh() {}

        ResourceMesh::~ResourceMesh() {
            qDeleteAll(meshSegments);
        }

        const QVector<Model::MeshSegment*>& ResourceMesh::getMeshSegments() const {
            return meshSegments;
        }

        void ResourceMesh::addMeshSegment(Model::MeshSegment *segment) {
            meshSegments.append(segment);
        }
    }
}

