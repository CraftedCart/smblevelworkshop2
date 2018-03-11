/**
 * @file
 * @brief Header for the WS2Editor::RenderManager class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RENDERMANAGER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RENDERMANAGER_HPP

#include "ws2common/model/MeshSegment.hpp"
#include "ws2editor/CachedGlMesh.hpp"
#include "ws2editor/CachedGlTexture.hpp"
#include "ws2editor/rendering/IRenderCommand.hpp"
#include <QFile>
#include <QQueue>
#include <QHash>

namespace WS2Editor {
    using namespace WS2Common::Model;
    using namespace WS2Common::Resource;
    using namespace WS2Editor::Rendering;

    /**
     * @brief Don't forget to call init after constructig this
     *
     * init isn't called in the constructor as the correct OpenGL context may not be bound then
     */
    class RenderManager {
        public:
            enum EnumVertexAttribs {
                VERTEX_POSITION = 0,
                VERTEX_NORMAL = 1,
                VERTEX_TEX_COORD = 2
            };

        protected:
            QQueue<IRenderCommand*> renderFifo;

            CachedGlTexture defaultTexture;

            /**
             * @brief Meshes that are currently being loaded
             *
             * This is used to ensure that meshes aren't loaded multiple times
             */
            QHash<const MeshSegment*, CachedGlMesh*> meshCache;
            QHash<const ResourceTexture*, CachedGlTexture*> textureCache;

        public:
            /**
             * @brief The maximum concurrently bound textures a shader can handle
             */
            const unsigned int MAX_SHADER_TEXTURES = 32;

            GLuint progID;
            GLuint shaderModelID;
            GLuint shaderViewID;
            GLuint shaderProjID;
            GLuint shaderNormID;
            GLuint shaderTexID;

            GLuint physicsDebugProgID;
            GLuint physicsDebugShaderModelID;
            GLuint physicsDebugShaderViewID;
            GLuint physicsDebugShaderProjID;

        protected:
            //Copied straight from Qt QGL
            static QImage convertToGLFormat(const QImage &img);
            static void convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format);
            static QRgb qt_gl_convertToGLFormatHelper(QRgb src_pixel, GLenum texture_format);

            void loadMesh(const MeshSegment *mesh);

            /**
             * @brief Loads a QImage texture into the GPU
             *
             * @param texture The texture to load into the GPU
             *
             * @return An object containing data about the loaded GL texture
             */
            CachedGlTexture* loadTexture(const QImage &texture);

            /**
             * @brief Loads a ResourceTexture to a GL texture
             *
             * The default texture will be loaded into the textureCache, before it is replaced with the provided texture
             * when it has loaded
             *
             * @param texture A loaded ResourceTexture
             */
            void loadTextureAsync(const ResourceTexture *texture);

            /**
             * @brief Finds occurances of placeholder text in shader strings, and replaces them with a constant
             *
             * @param s The string to find and replace in
             */
            void substituteShaderConstants(QString *s);

        public:
            void init();

            /**
             * @brief Load, compile and link the shader files given
             *
             * @param vertFile The vertex shader file
             * @param fragFile The fragment shader file
             *
             * @return The program ID obtained when linking the shaders
             *
             * @todo Handle errors
             */
            GLuint loadShaders(QFile *vertFile, QFile *fragFile);

            /**
             * @brief Adds a RenderMesh command to the render fifo, to be rendered later with renderQueue()
             *
             * @param mesh
             */
            void enqueueRenderMesh(const MeshSegment *mesh);

            /**
             * @brief Renders all meshes in the render fifo
             */
            void renderQueue();

            static void checkErrors(QString location);
    };
}

#endif

