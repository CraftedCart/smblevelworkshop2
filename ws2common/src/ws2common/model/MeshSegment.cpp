#include "ws2common/model/MeshSegment.hpp"

namespace WS2Common {
    namespace Model {
        MeshSegment::MeshSegment(
                QVector<Vertex> vertices,
                QVector<unsigned int> indices,
                QVector<Resource::ResourceTexture*> textures) {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            //Don't start the AABB at 0, 0, 0 (Not all object may intersect the world origin point)
            if (vertices.size() > 0) {
                aabb.a = vertices.at(0).position;
                aabb.b = vertices.at(0).position;
            }

            //Merge all verts into the AABB to make it fit this mesh segment
            for (Vertex &vert : vertices) {
                aabb.mergeWith(vert.position);
            }
        }

        MeshSegment::~MeshSegment() {}

        const QVector<Vertex>& MeshSegment::getVertices() const {
            return vertices;
        }

        const QVector<unsigned int>& MeshSegment::getIndices() const {
            return indices;
        }

        QVector<Resource::ResourceTexture*>& MeshSegment::getTextures() {
            return textures;
        }

        const QVector<Resource::ResourceTexture*>& MeshSegment::getTextures() const {
            return textures;
        }

        const AABB3& MeshSegment::getAabb() const {
            return aabb;
        }
    }
}

