#include "ws2editor/RenderManager.hpp"
#include "ws2editor/glplatform.hpp"
#include "ws2editor/GLManager.hpp"
#include "ws2editor/rendering/MeshRenderCommand.hpp"
#include <QElapsedTimer>
#include <QDebug>

namespace WS2Editor {
    void RenderManager::init() {
        //QImage defaultImage(":/Workshop2/Images/defaultgrid.png");
        //loadTexture(defaultImage);
    }

    //Copied straight from Qt QGL
    QImage RenderManager::convertToGLFormat(const QImage& img) {
        QImage res(img.size(), QImage::Format_ARGB32);
        convertToGLFormatHelper(res, img.convertToFormat(QImage::Format_ARGB32), GL_RGBA);
        return res;
    }

    //Also copied straight from Qt QGL
    void RenderManager::convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format) {
        Q_ASSERT(dst.depth() == 32);
        Q_ASSERT(img.depth() == 32);

        if (dst.size() != img.size()) {
            int target_width = dst.width();
            int target_height = dst.height();
            qreal sx = target_width / qreal(img.width());
            qreal sy = target_height / qreal(img.height());

            quint32 *dest = (quint32 *) dst.scanLine(0); // NB! avoid detach here
            uchar *srcPixels = (uchar *) img.scanLine(img.height() - 1);
            int sbpl = img.bytesPerLine();
            int dbpl = dst.bytesPerLine();

            int ix = int(0x00010000 / sx);
            int iy = int(0x00010000 / sy);

            quint32 basex = int(0.5 * ix);
            quint32 srcy = int(0.5 * iy);

            //Scale, swizzle and mirror in one loop
            while (target_height--) {
                const uint *src = (const quint32 *) (srcPixels - (srcy >> 16) * sbpl);
                int srcx = basex;
                for (int x=0; x<target_width; ++x) {
                    dest[x] = qt_gl_convertToGLFormatHelper(src[srcx >> 16], texture_format);
                    srcx += ix;
                }
                dest = (quint32 *)(((uchar *) dest) + dbpl);
                srcy += iy;
            }
        } else {
            const int width = img.width();
            const int height = img.height();
            const uint *p = (const uint*) img.scanLine(img.height() - 1);
            uint *q = (uint*) dst.scanLine(0);

            if (texture_format == GL_BGRA) {
                if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                    //Mirror + swizzle
                    for (int i=0; i < height; ++i) {
                        const uint *end = p + width;
                        while (p < end) {
                            *q = ((*p << 24) & 0xff000000)
                                | ((*p >> 24) & 0x000000ff)
                                | ((*p << 8) & 0x00ff0000)
                                | ((*p >> 8) & 0x0000ff00);
                            p++;
                            q++;
                        }
                        p -= 2 * width;
                    }
                } else {
                    const uint bytesPerLine = img.bytesPerLine();
                    for (int i=0; i < height; ++i) {
                        memcpy(q, p, bytesPerLine);
                        q += width;
                        p -= width;
                    }
                }
            } else {
                if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                    for (int i=0; i < height; ++i) {
                        const uint *end = p + width;
                        while (p < end) {
                            *q = (*p << 8) | ((*p >> 24) & 0xff);
                            p++;
                            q++;
                        }
                        p -= 2 * width;
                    }
                } else {
                    for (int i=0; i < height; ++i) {
                        const uint *end = p + width;
                        while (p < end) {
                            *q = ((*p << 16) & 0xff0000) | ((*p >> 16) & 0xff) | (*p & 0xff00ff00);
                            p++;
                            q++;
                        }
                        p -= 2 * width;
                    }
                }
            }
        }
    }

    //Also copied straight from Qt QGL
    QRgb RenderManager::qt_gl_convertToGLFormatHelper(QRgb src_pixel, GLenum texture_format) {
        if (texture_format == GL_BGRA) {
            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                return ((src_pixel << 24) & 0xff000000)
                    | ((src_pixel >> 24) & 0x000000ff)
                    | ((src_pixel << 8) & 0x00ff0000)
                    | ((src_pixel >> 8) & 0x0000ff00);
            } else {
                return src_pixel;
            }
        } else {  //GL_RGBA
            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {
                return (src_pixel << 8) | ((src_pixel >> 24) & 0xff);
            } else {
                return ((src_pixel << 16) & 0xff0000)
                    | ((src_pixel >> 16) & 0xff)
                    | (src_pixel & 0xff00ff00);
            }
        }
    }

    void RenderManager::loadMesh(const MeshSegment *mesh) {
        using namespace WS2Common::Model;

        GLuint vao;
        GLuint vbo;
        GLuint ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->getVertices().size() * sizeof(Vertex), &mesh->getVertices()[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndices().size() * sizeof(unsigned int), &mesh->getIndices()[0], GL_STATIC_DRAW);

        //Vertex positions
        glEnableVertexAttribArray(GLManager::EnumVertexAttribs::VERTEX_POSITION);
        glVertexAttribPointer(GLManager::EnumVertexAttribs::VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

        //Vertex normals
        glEnableVertexAttribArray(GLManager::EnumVertexAttribs::VERTEX_NORMAL);
        glVertexAttribPointer(GLManager::EnumVertexAttribs::VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

        //Vertex texture coordinates
        glEnableVertexAttribArray(GLManager::EnumVertexAttribs::VERTEX_TEX_COORD);
        glVertexAttribPointer(GLManager::EnumVertexAttribs::VERTEX_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));

        glBindVertexArray(0);

        CachedGlMesh *cachedMesh = new CachedGlMesh;
        cachedMesh->setVao(vao);
        cachedMesh->setVbo(vbo);
        cachedMesh->setEbo(ebo);
        cachedMesh->setTriCount(mesh->getIndices().size());

        //Find the textures, and laod them if they aren't cached already
        foreach(const ResourceTexture *tex, mesh->getTextures()) {
            if (!textureCache.contains(tex)) loadTextureAsync(tex);
            cachedMesh->getTextures().append(textureCache[tex]);
        }

        meshCache[mesh] = cachedMesh;
    }

    CachedGlTexture* RenderManager::loadTexture(const QImage &texture) {
        QImage rgbaImage = convertToGLFormat(texture);

        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rgbaImage.width(), rgbaImage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaImage.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        checkErrors("RenderManager::loadTexture");

        CachedGlTexture *cachedTex = new CachedGlTexture;
        cachedTex->setTextureId(texId);

        return cachedTex;
    }

    void RenderManager::loadTextureAsync(const ResourceTexture *texture) {
        //TODO: Actually make this async

        QImage img;
        if (texture->isLoaded()) {
            img = *texture->getTexture();
        } else {
            img = QImage(*texture->getFirstFilePath());
        }
        CachedGlTexture *tex = loadTexture(img);

        textureCache[texture] = tex;
    }

    void RenderManager::enqueueRenderMesh(const MeshSegment *mesh) {
        //First check if the mesh has been cached already
        //If it hasen't, we need to load it first
        if (!meshCache.contains(mesh)) loadMesh(mesh);

        //TODO: Update cache access time thingy
        renderFifo.enqueue(new MeshRenderCommand(meshCache[mesh]));
    }

    void RenderManager::renderQueue() {
        using namespace WS2Editor::Rendering;

        foreach(IRenderCommand *cmd, renderFifo) cmd->draw();
        qDeleteAll(renderFifo);
        renderFifo.clear();
    }

    void RenderManager::checkErrors(QString location) {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            //Errors occured
            QString errString;

            switch(err) {
                case GL_INVALID_OPERATION:
                    errString = "GL_INVALID_OPERATION";
                    break;
                case GL_INVALID_ENUM:
                    errString = "GL_INVALID_ENUM";
                    break;
                case GL_INVALID_VALUE:
                    errString = "GL_INVALID_VALUE";
                    break;
                case GL_OUT_OF_MEMORY:
                    errString = "GL_OUT_OF_MEMORY";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    errString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                    break;
            }

            qWarning() << "GL Error:" << err << "-" << errString << "- Found at:" << location;
        }
    }
}

