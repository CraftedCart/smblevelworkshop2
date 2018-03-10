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

        public:
            void init();

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

