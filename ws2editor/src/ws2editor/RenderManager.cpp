#include "ws2editor/RenderManager.hpp"
#include "ws2editor/glplatform.hpp"
#include "ws2editor/rendering/MeshRenderCommand.hpp"
#include "ws2editor/task/LoadGlTextureTask.hpp"
#include "ws2editor/task/TaskManager.hpp"
#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/WS2Editor.hpp"
#include <glm/gtx/transform.hpp>
#include <QElapsedTimer>
#include <QDebug>

namespace WS2Editor {
    void RenderManager::init(int fboWidth, int fboHeight) {
        qDebug() << "Initializing RenderManager";

        viewportWidth = fboWidth;
        viewportHeight = fboHeight;

        qDebug() << "Loading default texture";
        QImage defaultImage = convertToGLFormat(QImage(":/Workshop2/Images/defaultgrid.png"));
        defaultTexture = loadTexture(defaultImage);

        //Create fbo
        qDebug() << "Creating framebuffers";
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        //Create fbo textures/buffers
        generateFboAttachments(fboWidth, fboHeight);

        //Generate the fullscreen quad buffers, to draw the framebuffer
        qDebug() << "Creating fullscreen quad buffers";
        glGenVertexArrays(1, &fullscreenQuadVao);
        glBindVertexArray(fullscreenQuadVao);

        static const GLfloat fullscreenQuadVerts[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
        };

        glGenBuffers(1, &fullscreenQuadVbo);
        glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuadVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fullscreenQuadVerts), fullscreenQuadVerts, GL_STATIC_DRAW);

        glEnableVertexAttribArray(EnumVertexAttribs::VERTEX_POSITION);
        glVertexAttribPointer(EnumVertexAttribs::VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        qDebug() << "Creating shaders";

        //Load the shaders
        QFile stageVertFile(":/Workshop2/Shaders/stage.vert");
        QFile stageFragFile(":/Workshop2/Shaders/stage.frag");
        progID = loadShaders(&stageVertFile, &stageFragFile);

        //Get uniform IDs
        shaderModelID = glGetUniformLocation(progID, "modelMat");
        shaderViewID = glGetUniformLocation(progID, "viewMat");
        shaderProjID = glGetUniformLocation(progID, "projMat");
        shaderNormID = glGetUniformLocation(progID, "normMat");
        //shaderTexID = glGetUniformLocation(progID, "texSampler");
        shaderTexInfluenceID = glGetUniformLocation(progID, "texInfluence");
        shaderTintID = glGetUniformLocation(progID, "tint");
        shaderRenderCameraNormals = glGetUniformLocation(progID, "renderCameraNormals");

        //Load the physics debug shaders
        //TODO: Make loading this an option
        //QFile physVertFile(":/Workshop2/Shaders/physicsDebug.vert");
        //QFile physFragFile(":/Workshop2/Shaders/physicsDebug.frag");
        //physicsDebugProgID = GLManager::loadShaders(&physVertFile, &physFragFile);

        //Get uniform IDs
        //physicsDebugShaderViewID = glGetUniformLocation(physicsDebugProgID, "viewMat");
        //physicsDebugShaderProjID = glGetUniformLocation(physicsDebugProgID, "projMat");

        QFile compositeVertFile(":/Workshop2/Shaders/composite.vert");
        QFile compositeFragFile(":/Workshop2/Shaders/composite.frag");
        compositeShaderProg = loadShaders(&compositeVertFile, &compositeFragFile);
        compositeShaderTextureId = glGetUniformLocation(compositeShaderProg, "texSampler");
        compositeShaderCameraNormalsTextureId = glGetUniformLocation(compositeShaderProg, "cameraNormalsTexSampler");

        checkErrors("After RenderManager::init()");
    }

    void RenderManager::destroy() {
        clearAllCaches(); //Unload all render objects
        unloadShaders();
        //unloadPhysicsDebugShaders();

        GLuint texturesToDelete[] = {defaultTexture->getTextureId(), fboColorTexture, fboCameraNormalTexture};
        glDeleteTextures(3, texturesToDelete);
        delete defaultTexture;

        glDeleteRenderbuffers(1, &fboDepthBuffer);
        glDeleteFramebuffers(1, &fbo);

        glDeleteBuffers(1, &fullscreenQuadVbo);
        glDeleteVertexArrays(1, &fullscreenQuadVao);

        checkErrors("End of RenderManager::destroy()");
    }

    void RenderManager::generateFboAttachments(int fboWidth, int fboHeight) {
        //Color texture
        glGenTextures(1, &fboColorTexture);
        glBindTexture(GL_TEXTURE_2D, fboColorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fboColorTexture, 0);

        //Camera normal texture
        glGenTextures(1, &fboCameraNormalTexture);
        glBindTexture(GL_TEXTURE_2D, fboCameraNormalTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, fboCameraNormalTexture, 0);

        //Depth buffer
        glGenRenderbuffers(1, &fboDepthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, fboDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fboWidth, fboHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fboDepthBuffer);

        //Set the draw buffers list
        GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, drawBuffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            //TODO: Throw an exception or something
            qCritical() << "fbo is incomplete";
        }
    }

    void RenderManager::resizeViewport(int width, int height) {
        viewportWidth = width;
        viewportHeight = height;

        //destroyFboAttachments();
        //generateFboAttachments(width, height);
        //Color texture
        glBindTexture(GL_TEXTURE_2D, fboColorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        //Camera normal texture
        glBindTexture(GL_TEXTURE_2D, fboCameraNormalTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        //Depth buffer
        glBindRenderbuffer(GL_RENDERBUFFER, fboDepthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
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

            quint32 *dest = (quint32 *) dst.scanLine(0); //NB! avoid detach here
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
        glEnableVertexAttribArray(EnumVertexAttribs::VERTEX_POSITION);
        glVertexAttribPointer(EnumVertexAttribs::VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

        //Vertex normals
        glEnableVertexAttribArray(EnumVertexAttribs::VERTEX_NORMAL);
        glVertexAttribPointer(EnumVertexAttribs::VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

        //Vertex texture coordinates
        glEnableVertexAttribArray(EnumVertexAttribs::VERTEX_TEX_COORD);
        glVertexAttribPointer(EnumVertexAttribs::VERTEX_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));

        glBindVertexArray(0);

        CachedGlMesh *cachedMesh = new CachedGlMesh;
        cachedMesh->setVao(vao);
        cachedMesh->setVbo(vbo);
        cachedMesh->setEbo(ebo);
        cachedMesh->setTriCount(mesh->getIndices().size());

        //Add the textures
        foreach(const ResourceTexture *tex, mesh->getTextures()) {
            cachedMesh->getTextures().append(tex);
        }

        cachedMesh->updateAccessTimer();

        meshCache[mesh] = cachedMesh;
    }

    CachedGlTexture* RenderManager::loadTexture(const QImage &texture) {
        GLuint texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        checkErrors("RenderManager::loadTexture");

        CachedGlTexture *cachedTex = new CachedGlTexture;
        cachedTex->setTextureId(texId);

        cachedTex->updateAccessTimer();

        return cachedTex;
    }

    void RenderManager::loadTextureAsync(const ResourceTexture *texture) {
        using namespace WS2Editor::Task;

        qDebug().noquote() << "Loading GL texture:" << *texture->getFirstFilePath();

        //TODO: Actually make this async

        QImage img;
        if (texture->isLoaded()) {
            img = *texture->getTexture();
        } else {
            img = QImage(*texture->getFirstFilePath());

            LoadGlTextureTask *task = new LoadGlTextureTask(*texture);
            connect(task, &LoadGlTextureTask::addTexture, this, &RenderManager::addTexture);
            ws2TaskManager->enqueueTask(task);

            textureCache[texture] = defaultTexture;
            return;
        }

        CachedGlTexture *tex = loadTexture(convertToGLFormat(img));

        textureCache[texture] = tex;
    }

    GLuint RenderManager::loadShaders(QFile *vertFile, QFile *fragFile) {
        //Create the shaders
        GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

        //Read the vert shader code
        if (!vertFile->open(QFile::ReadOnly)) {
            //Failed to open the file
            //TODO: Handle failing to open file
            qWarning() << "Failed to open file:" << vertFile->fileName();
            return 0;
        }
        QString vertCode = vertFile->readAll();
        vertFile->close();

        //Read the frag shader code
        if (!fragFile->open(QFile::ReadOnly)) {
            //Failed to open the file
            //TODO: Handle failing to open file
            qWarning() << "Failed to open file:" << fragFile->fileName();
            return 0;
        }
        QString fragCode = fragFile->readAll();
        fragFile->close();

        GLint result = GL_FALSE;
        int infoLogLength;

        QByteArray vertBytes = vertCode.toUtf8();
        const char *cVertCode = vertBytes.constData();
        QByteArray fragBytes = fragCode.toUtf8();
        const char *cFragCode = fragBytes.constData();

        //Compile vert shader
        qDebug() << "Compiling vertex shader:" << vertFile->fileName();
        glShaderSource(vertID, 1, &cVertCode, NULL);
        glCompileShader(vertID);

        //Check vert shader
        glGetShaderiv(vertID, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::vector<char> vertShaderErrMsg(infoLogLength + 1);
            glGetShaderInfoLog(vertID, infoLogLength, NULL, &vertShaderErrMsg[0]);
            qWarning() << &vertShaderErrMsg[0];

            //TODO: Handle error
        }

        //Compile frag shader
        qDebug() << "Compiling fragment shader:" << fragFile->fileName();
        glShaderSource(fragID, 1, &cFragCode, NULL);
        glCompileShader(fragID);

        //Check frag shader
        glGetShaderiv(fragID, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::vector<char> fragShaderErrMsg(infoLogLength + 1);
            glGetShaderInfoLog(fragID, infoLogLength, NULL, &fragShaderErrMsg[0]);
            qWarning() << &fragShaderErrMsg[0];

            //TODO: Handle error
        }

        //Link the program
        qDebug() << "Linking program";
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertID);
        glAttachShader(programID, fragID);
        glLinkProgram(programID);

        //Check program
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        if (result == GL_FALSE) {
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::vector<char> progErrMsg(infoLogLength + 1);
            glGetProgramInfoLog(programID, infoLogLength, NULL, &progErrMsg[0]);
            qWarning() << &progErrMsg[0];

            //TODO: Handle error
        }

        //Cleanup
        glDetachShader(programID, vertID);
        glDetachShader(programID, fragID);

        glDeleteShader(vertID);
        glDeleteShader(fragID);

        return programID;
    }

    void RenderManager::enqueueRenderScene(SceneNode *rootNode, const Scene::SceneSelectionManager *selectionManager) {
        recursiveEnqueueSceneNode(rootNode, selectionManager, glm::mat4(1.0f));
    }

    void RenderManager::recursiveEnqueueSceneNode(
            WS2Common::Scene::SceneNode *node,
            const Scene::SceneSelectionManager *selectionManager,
            const glm::mat4 parentTransform) {
        using namespace WS2Editor::Scene;

        glm::mat4 transform = parentTransform;
        transform = glm::translate(transform, node->getPosition());
        transform = glm::rotate(transform, node->getRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, node->getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, node->getRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, node->getScale());

        //Check if this is a renderable object, and enqueue it for rendering if so
        if (const EditorMeshSceneNode *mesh = dynamic_cast<const EditorMeshSceneNode*>(node)) {
            const QVector<WS2Common::Model::MeshSegment*>& segments = mesh->getMesh()->getMeshSegments();
            for (int i = 0; i < segments.size(); i++) {
                bool isSelected = selectionManager->isSelected(node);
                enqueueRenderMesh(segments[i], transform, isSelected);
            }
        }

        //Check children to recirsively render
        for (int i = 0; i < node->getChildren().size(); i++) {
            recursiveEnqueueSceneNode(node->getChildren().at(i), selectionManager, transform);
        }
    }

    void RenderManager::enqueueRenderMesh(const MeshSegment *mesh, glm::mat4 transform, bool renderCameraNormals) {
        //First check if the mesh has been cached already
        //If it hasen't, we need to load it first
        if (!meshCache.contains(mesh)) loadMesh(mesh);

        renderFifo.enqueue(new MeshRenderCommand(meshCache[mesh], this, transform, glm::vec4(1.0f), renderCameraNormals));
    }

    void RenderManager::renderQueue(GLuint targetFramebuffer) {
        using namespace WS2Editor::Rendering;

        //Bind the FBO
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, viewportWidth, viewportHeight);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Render everything to the FBO
        foreach(IRenderCommand *cmd, renderFifo) cmd->draw();

        //Render the FBO contents to the screen
        glBindFramebuffer(GL_FRAMEBUFFER, targetFramebuffer);
        glViewport(0, 0, viewportWidth, viewportHeight);

        glUseProgram(compositeShaderProg);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fboColorTexture);
        glUniform1i(compositeShaderTextureId, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, fboCameraNormalTexture);
        glUniform1i(compositeShaderCameraNormalsTextureId, 1);

        glBindVertexArray(fullscreenQuadVao);

        //Draw the triangles
        glDrawArrays(GL_TRIANGLES, 0, 6); //6 verts for 2 tris (As it's a fullscreen **quad**)

        //glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFramebuffer);
        //glBlitFramebuffer(0, 0, 1920, 1080, 0, 0, 1920, 1080, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        //Clear the fifo
        qDeleteAll(renderFifo);
        renderFifo.clear();

        //Check for timed-out cached objects
        unsigned int deletedMeshes = 0;
        unsigned int deletedTextures = 0;

        QMutableHashIterator<const MeshSegment*, CachedGlMesh*> meshTimeoutIter(meshCache);
        while (meshTimeoutIter.hasNext()) {
            meshTimeoutIter.next();
            if (meshTimeoutIter.value()->getLastAccessTimer().elapsed() > CACHE_TIMEOUT) {
                GLuint vao = meshTimeoutIter.value()->getVao();
                GLuint buffers[] {
                    meshTimeoutIter.value()->getVbo(),
                    meshTimeoutIter.value()->getEbo()
                };

                glDeleteVertexArrays(1, &vao);
                glDeleteBuffers(2, buffers);

                delete meshTimeoutIter.value();
                meshTimeoutIter.remove();

                deletedMeshes++;
            }
        }

        QMutableHashIterator<const ResourceTexture*, CachedGlTexture*> texTimeoutIter(textureCache);
        while (texTimeoutIter.hasNext()) {
            texTimeoutIter.next();
            if (texTimeoutIter.value()->getLastAccessTimer().elapsed() > CACHE_TIMEOUT) {
                GLuint texId = texTimeoutIter.value()->getTextureId();

                glDeleteTextures(1, &texId);

                delete texTimeoutIter.value();
                texTimeoutIter.remove();

                deletedTextures++;
            }
        }

        if (deletedMeshes > 0) qDebug().noquote() << QString("Purged %1 mesh(es) from the RenderManager cache").arg(deletedMeshes);
        if (deletedTextures > 0) qDebug().noquote() << QString("Purged %1 texture(s) from the RenderManager cache").arg(deletedTextures);
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

    void RenderManager::clearMeshCache() {
        qDebug().noquote() << QString("Clearing %1 cached meshes").arg(meshCache.size());

        //Unload all meshes
        GLuint *vertexArrays = new GLuint[meshCache.size()];
        GLuint *buffers = new GLuint[meshCache.size() * 2];

        unsigned int i = 0;
        foreach(CachedGlMesh *mesh, meshCache.values()) {
            vertexArrays[i] = mesh->getVao();
            buffers[i * 2] = mesh->getVbo();
            buffers[i * 2 + 1] = mesh->getEbo();

            i++;
        }

        glDeleteVertexArrays(meshCache.size(), vertexArrays);
        glDeleteBuffers(meshCache.size() * 2, buffers);

        delete[] vertexArrays;
        delete[] buffers;

        qDeleteAll(meshCache.values());
        meshCache.clear();
    }

    void RenderManager::clearTextureCache() {
        qDebug().noquote() << QString("Clearing %1 cached textures").arg(textureCache.size());

        //Unload all meshes
        GLuint *textures = new GLuint[textureCache.size()];

        unsigned int i = 0;
        foreach(CachedGlTexture *tex, textureCache.values()) {
            textures[i] = tex->getTextureId();

            i++;
        }

        glDeleteTextures(textureCache.size(), textures);

        delete[] textures;

        qDeleteAll(textureCache.values());
        textureCache.clear();
    }

    void RenderManager::clearAllCaches() {
        clearMeshCache();
        clearTextureCache();
    }

    CachedGlTexture* RenderManager::getTextureForResourceTexture(const ResourceTexture *tex) {
        //First check if the texture has been cached already
        //If it hasen't, we need to load it first
        if (!textureCache.contains(tex)) {
            loadTextureAsync(tex);
            //TODO: Return default texture
        }

        //TODO: Update cache access time thingy
        return textureCache[tex];
    }

    void RenderManager::unloadShaders() {
        glDeleteProgram(progID);
        glDeleteProgram(compositeShaderProg);
    }

    void RenderManager::addTexture(const QImage image, const ResourceTexture *tex) {
        textureCache[tex] = loadTexture(image);
    }

}
