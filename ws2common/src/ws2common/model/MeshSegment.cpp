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
    }
}

