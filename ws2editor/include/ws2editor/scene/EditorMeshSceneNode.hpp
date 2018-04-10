/**
 * @file
 * @brief Header for the WS2Editor::Scene::EditorMeshSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_SCENE_MESHSCEHENODE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_SCENE_MESHSCEHENODE_HPP

#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <btBulletDynamicsCommon.h>

namespace WS2Editor {
    namespace Resource {
        class ResourceScene;
    }

    namespace Scene {
        using namespace WS2Editor::Resource;
        using namespace WS2Common::Resource;

        /**
         * @brief Extends on the MeshSceneNode, providing extra functionality to integrate it into WS2Editor, such as
         *        a renderable mesh and collision shape for raycasting.
         */
        class EditorMeshSceneNode : public WS2Common::Scene::MeshSceneNode {
            protected:
                ResourceMesh *mesh;
                ResourceScene *scene;
                btCollisionShape *physicsCollisionShape;
                btDefaultMotionState *physicsMotionState;
                btRigidBody *physicsRigidBody;

                /**
                 * @brief Initializes physics for the mesh
                 */
                void initPhysics();

            public:
                /**
                 * @brief Constructs a EditorMeshSceneNode with no mesh
                 *
                 * @note This will init physics, but the rigid body is **not** added to any dynamics world.
                 *       Use `getRigidBody()` to get the rigid body, and add it to a dynamics world.
                 *
                 * @param name The name of the node
                 * @param scene The scene this mesh is a part of
                 */
                EditorMeshSceneNode(const QString name, ResourceScene *scene);

                /**
                 * @brief Constructs a EditorMeshSceneNode with a mesh
                 *
                 * @note This will init physics, but the rigid body is **not** added to any dynamics world.
                 *       Use `getRigidBody()` to get the rigid body, and add it to a dynamics world.
                 *
                 * @param name The name of the node
                 * @param scene The scene this mesh is a part of
                 * @param mesh A pointer to the mesh to set
                 */
                EditorMeshSceneNode(const QString name, ResourceScene *scene, WS2Common::Resource::ResourceMesh *mesh);

                /**
                 * @brief Frees up resources
                 */
                ~EditorMeshSceneNode();

                /**
                 * @brief Getter for WS2Editor::Scene::EditorMeshSceneNode::mesh
                 *
                 * @return A pointer to the mesh
                 */
                const WS2Common::Resource::ResourceMesh* getMesh() const;

                /**
                 * @brief Getter for WS2Editor::Scene::EditorMeshSceneNode::physicsRigidBody
                 *
                 * @return A pointer to this node's rigid body
                 */
                btRigidBody* getPhysicsRigidBody();
        };
    }
}

#endif

