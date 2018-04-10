/**
 * @file
 * @brief Header for the WS2Editor::Model::Scene class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCESCENE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCESCENE_HPP

#include "ws2editor/scene/SceneSelectionManager.hpp"
#include "ws2editor/physics/PhysicsManger.hpp"
#include "ws2editor/PhysicsDebugDrawer.hpp"
#include "ws2common/resource/AbstractResource.hpp"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QFile>

namespace WS2Editor {
    namespace Resource {
        using namespace WS2Common::Resource;

        /**
         * @todo Meshes loaded should belong to ResourceManager, not ResourceScene
         */
        class ResourceScene : public WS2Common::Resource::AbstractResource {
            Q_OBJECT

            protected:
                WS2Common::Scene::SceneNode *rootNode;
                Scene::SceneSelectionManager *selectionManager;
                Physics::PhysicsManager *physicsManager;
                PhysicsDebugDrawer *physicsDebugDrawer = nullptr;

            public:
                /**
                 * @brief Constructs an empty scene with a SceneNode named `tr("Static")`
                 */
                ResourceScene();

                /**
                 * @brief Frees up resources
                 */
                ~ResourceScene();

                /**
                 * @brief Creates and sets the physics debug drawer
                 *
                 * @note This needs to be done when the correct OpenGL context is bound
                 */
                void initPhysicsDebugDrawer();

                /**
                 * @brief Getter for physicsDebugDrawer
                 *
                 * @return A pointer to the physics debug drawer, or nullptr if it hasn't been initialized
                 */
                PhysicsDebugDrawer* getPhysicsDebugDrawer();

                /**
                 * @brief Loads all models from the filePaths vector
                 */
                void load() override;

                /**
                 * @brief Unloads all models
                 */
                void unload() override;

                /**
                 * @brief Getter for rootNode
                 *
                 * @return This scene's root node of the scenegraph
                 */
                WS2Common::Scene::SceneNode* getRootNode();

                /**
                 * @brief Getter for selectionManager
                 *
                 * @return This scene's selection manager
                 */
                Scene::SceneSelectionManager* getSelectionManager();

                /**
                 * @brief Getter for physicsManager
                 *
                 * @return This scene's physics manager
                 */
                Physics::PhysicsManager* getPhysicsManager();

            public slots:
                /**
                 * @brief Appends models to the scene from the mesh vector given
                 *
                 * @param meshes The meshes to append
                 *
                 * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
                 */
                void addModel(const QVector<ResourceMesh*> &meshes);

                /**
                 * @brief Slot for when the selected nodes has been changed
                 *
                 * @param selectedObjects The new selected nodes vector
                 */
                void onSelectionChanged(QVector<WS2Common::Scene::SceneNode*>& selectedObjects);

        };
    }
}

#endif

