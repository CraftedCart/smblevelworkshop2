/**
 * @file
 * @brief Header for the WS2Editor::Scene::EditorMeshSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_SCENE_MESHSCEHENODE_HPP
#define SMBLEVELWORKSHOP2_SCENE_MESHSCEHENODE_HPP

#include "scene/MeshSceneNode.hpp"
#include "resource/ResourceMesh.hpp"
#include <btBulletDynamicsCommon.h>

namespace WS2Editor {
    namespace Scene {

        /**
         * @brief Extends on the MeshSceneNode, providing extra functionality to integrate it into WS2Editor, such as
         *        a renderable mesh and collision shape for raycasting.
         */
        class EditorMeshSceneNode : public WS2Common::Scene::MeshSceneNode {
            protected:
                Resource::ResourceMesh *mesh;
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
                 */
                EditorMeshSceneNode(const QString name);

                /**
                 * @brief Constructs a EditorMeshSceneNode with a mesh
                 *
                 * @note This will init physics, but the rigid body is **not** added to any dynamics world.
                 *       Use `getRigidBody()` to get the rigid body, and add it to a dynamics world.
                 *
                 * @param name The name of the node
                 * @param mesh A pointer to the mesh to set
                 */
                EditorMeshSceneNode(const QString name, Resource::ResourceMesh *mesh);

                /**
                 * @brief Frees up resources
                 */
                ~EditorMeshSceneNode();

                /**
                 * @brief Getter for WS2Editor::Scene::EditorMeshSceneNode::mesh
                 *
                 * @return A pointer to the mesh
                 */
                const Resource::ResourceMesh* getMesh() const;

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

