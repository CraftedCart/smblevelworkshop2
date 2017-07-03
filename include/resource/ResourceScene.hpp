/**
 * @file
 * @brief Header for the WS2::Model::Scene class
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCE_RESOURCESCENE_HPP
#define SMBLEVELWORKSHOP2_RESOURCE_RESOURCESCENE_HPP

#include "resource/ResourceMesh.hpp"
#include "resource/AbstractResource.hpp"
#include <QFile>

namespace WS2 {
    namespace Resource {
        /**
         * @todo Meshes loaded should belong to ResourceManager, not ResourceScene
         */
        class ResourceScene : public AbstractResource {
            protected:
                /**
                 * @brief A vector of 3D Meshes in the Scene
                 */
                QVector<Resource::ResourceMesh*> meshes;

            public:
                /**
                 * @brief Constructs an empty scene
                 */
                ResourceScene();

                /**
                 * @brief Construct a scene from the file given
                 *
                 * This just calls `addModel(file)` for you
                 *
                 * @param file The model file
                 */
                ResourceScene(QFile &file);

                /**
                 * @brief Loads all models from the filePaths vector
                 */
                void load() override;

                /**
                 * @brief Unloads all models
                 */
                void unload() override;

                /**
                 * @return The meshes that belong to this scene
                 */
                QVector<Resource::ResourceMesh*>& getMeshes();

                /**
                 * @brief Adds the file path to the resource filePaths vector, and append a model to the scene from the
                 *        file given if the resource is marked as loaded.
                 *
                 * @param file The model file to append
                 */
                void addModel(QFile &file);
        };
    }
}

#endif

