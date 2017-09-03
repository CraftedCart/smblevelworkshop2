/**
 * @file
 * @brief Header for the WS2Editor::Scene::MeshSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_SCENE_MESHSCEHENODE_HPP
#define SMBLEVELWORKSHOP2_SCENE_MESHSCEHENODE_HPP

#include "scene/SceneNode.hpp"
#include "resource/ResourceMesh.hpp"
#include <btBulletDynamicsCommon.h>

namespace WS2Editor {
    namespace Scene {
        class MeshSceneNode : public WS2Common::Scene::SceneNode {
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
                 * @brief Constructs a MeshSceneNode with no mesh
                 *
                 * @note This will init physics, but the rigid body is **not** added to any dynamics world.
                 *       Use `getRigidBody()` to get the rigid body, and add it to a dynamics world.
                 *
                 * @param name The name of the node
                 */
                MeshSceneNode(const QString name);

                /**
                 * @brief Constructs a MeshSceneNode with a mesh
                 *
                 * @note This will init physics, but the rigid body is **not** added to any dynamics world.
                 *       Use `getRigidBody()` to get the rigid body, and add it to a dynamics world.
                 *
                 * @param name The name of the node
                 * @param mesh A pointer to the mesh to set
                 */
                MeshSceneNode(const QString name, Resource::ResourceMesh *mesh);

                /**
                 * @brief Frees up resources
                 */
                ~MeshSceneNode();

                /**
                 * @brief Getter for WS2Editor::Scene::MeshSceneNode::mesh
                 *
                 * @return A pointer to the mesh
                 */
                const Resource::ResourceMesh* getMesh() const;

                /**
                 * @brief Getter for WS2Editor::Scene::MeshSceneNode::physicsRigidBody
                 *
                 * @return A pointer to this node's rigid body
                 */
                btRigidBody* getPhysicsRigidBody();
        };
    }
}

#endif

