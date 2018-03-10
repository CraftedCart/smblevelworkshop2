#include "ws2editor/CachedGlMesh.hpp"

namespace WS2Editor {
    void CachedGlMesh::setVao(GLuint vao) {
        this->vao = vao;
    }

    GLuint CachedGlMesh::getVao() const {
        return vao;
    }

    void CachedGlMesh::setVbo(GLuint vbo) {
        this->vbo = vbo;
    }

    GLuint CachedGlMesh::getVbo() const {
        return vbo;
    }

    void CachedGlMesh::setEbo(GLuint ebo) {
        this->ebo = ebo;
    }

    GLuint CachedGlMesh::getEbo() const {
        return ebo;
    }

    void CachedGlMesh::setTriCount(GLuint triCount) {
        this->triCount = triCount;
    }

    GLuint CachedGlMesh::getTriCount() const {
        return triCount;
    }

    QVector<WS2Common::Resource::ResourceTexture*>& CachedGlMesh::getTextures() {
        return textures;
    }
}

