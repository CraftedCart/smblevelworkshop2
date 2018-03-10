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

    class RenderManager {
        protected:
            QQueue<IRenderCommand*> renderFifo;

            /**
             * @brief Meshes that are currently being loaded
             *
             * This is used to ensure that meshes aren't loaded multiple times
             */
            QHash<const MeshSegment*, CachedGlMesh*> meshCache;
            QHash<const ResourceTexture*, CachedGlTexture*> textureCache;

            void loadMesh(const MeshSegment *mesh);

        public:
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
    };
}

#endif

