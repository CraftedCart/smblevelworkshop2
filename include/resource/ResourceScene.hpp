/**
 * @file
 * @brief Header for the WS2::Model::Scene class
 */

#ifndef SMBLEVELWORKSHOP2_RESOURCE_RESOURCESCENE_HPP
#define SMBLEVELWORKSHOP2_RESOURCE_RESOURCESCENE_HPP

#include "resource/ResourceMesh.hpp"
#include "resource/AbstractResource.hpp"
#include "scene/SceneNode.hpp"
#include <QFile>

namespace WS2 {
    namespace Resource {
        /**
         * @todo Meshes loaded should belong to ResourceManager, not ResourceScene
         */
        class ResourceScene : public AbstractResource {
            protected:
                Scene::SceneNode *rootNode = new Scene::SceneNode();

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

                Scene::SceneNode* getRootNode();

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

