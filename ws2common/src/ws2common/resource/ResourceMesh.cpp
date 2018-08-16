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

            //Don't start the AABB at 0, 0, 0 (Not all object may intersect the world origin point)
            //So set the AABB if this is the first segment being added
            if (meshSegments.size() == 1) {
                aabb = segment->getAabb();
            }

            aabb.mergeWith(segment->getAabb());
        }

        const AABB3& ResourceMesh::getAabb() const {
            return aabb;
        }
    }
}

