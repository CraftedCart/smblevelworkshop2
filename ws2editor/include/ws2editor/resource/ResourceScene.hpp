/**
 * @file
 * @brief Header for the WS2Editor::Model::Scene class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCESCENE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_RESOURCE_RESOURCESCENE_HPP

#include "ws2editor_export.h"
#include "ws2editor/scene/SceneSelectionManager.hpp"
#include "ws2editor/physics/PhysicsManager.hpp"
#include "ws2editor/PhysicsDebugDrawer.hpp"
#include "ws2editor/MeshNodeData.hpp"
#include "ws2common/resource/AbstractResource.hpp"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QFile>

namespace WS2Editor {
    namespace Resource {
        /**
         * @todo Meshes loaded should belong to ResourceManager, not ResourceScene
         */
        class WS2EDITOR_EXPORT ResourceScene : public WS2Common::Resource::AbstractResource {
            Q_OBJECT

            protected:
                WS2Common::Scene::SceneNode *rootNode;
                Scene::SceneSelectionManager *selectionManager;
                Physics::PhysicsManager *physicsManager;
                PhysicsDebugDrawer *physicsDebugDrawer = nullptr;

                /**
                 * @brief This ties nodes to mesh data - used for rendering and ray tracing
                 *
                 * The key identifies a node by UUID
                 */
                QHash<const QUuid, MeshNodeData*> nodeMeshData;

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
                 * @brief Fetches the first static node it can find, or nullptr if one doesn't exist
                 *
                 * @return This scene's first static node, or nullptr is there are none
                 */
                WS2Common::Scene::SceneNode* getStaticNode();

                /**
                 * @brief Getter for selectionManager
                 *
                 * @return This scene's selection manager
                 */
                Scene::SceneSelectionManager* getSelectionManager();

                /**
                 * @brief Const getter for selectionManager
                 *
                 * @return This scene's selection manager
                 */
                const Scene::SceneSelectionManager* getSelectionManager() const;

                /**
                 * @brief Getter for physicsManager
                 *
                 * @return This scene's physics manager
                 */
                Physics::PhysicsManager* getPhysicsManager();

                /**
                 * @brief Stores mesh node data in the scene and registers it with the scene physics manager
                 *
                 * @param uuid The UUID of the node to store data about
                 * @param data The mesh data to store
                 */
                void addMeshNodeData(const QUuid &uuid, MeshNodeData *data);

                /**
                 * @brief Removes and deletes stored mesh node data from the scene and unregisters it from the scene's
                 *        physics manager, or does nothing if the node doesn't have mesh data
                 *
                 * @param uuid The UUID of the node to remove stored data about
                 *
                 * @return Whether the node was found and therefore removed or not
                 */
                bool removeMeshNodeData(const QUuid &uuid);

                /**
                 * @brief Retrieves mesh data for a given node
                 *
                 * @param uuid The UUID of the node to retieve data about
                 *
                 * @return The mesh data for the node given, or nullptr is no mesh data is stored for the given node
                 */
                MeshNodeData* getMeshNodeData(const QUuid &uuid);

                /**
                 * @brief Retrieves mesh data for a given node - const edition
                 *
                 * @param uuid The UUID of the node to retieve data about
                 *
                 * @return The mesh data for the node given, or nullptr is no mesh data is stored for the given node
                 */
                const MeshNodeData* getMeshNodeData(const QUuid &uuid) const;

            public slots:
                /**
                 * @brief Appends models to the scene from the mesh vector given
                 *
                 * @param meshes The meshes to append
                 *
                 * @throws WS2Editor::Exception::RuntimeException When Assimp fails to generate an aiScene
                 */
                void addModel(const QVector<WS2Common::Resource::ResourceMesh*> &meshes);

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

