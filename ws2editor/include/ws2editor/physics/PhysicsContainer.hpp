/**
 * @file
 * @brief Header for the PhysicsContainer class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PHYSICS_PHYSICSCONTAINER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PHYSICS_PHYSICSCONTAINER_HPP

#include "ws2editor_export.h"
#include "ws2common/resource/ResourceMesh.hpp"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/Transform.hpp"
#include <btBulletDynamicsCommon.h>

namespace WS2Editor {
    namespace Physics {
        class WS2EDITOR_EXPORT PhysicsContainer {
            protected:
                btCollisionShape *physicsCollisionShape;
                btDefaultMotionState *physicsMotionState;
                btRigidBody *physicsRigidBody;

            public:
                /**
                 * @brief Constructor for PhysicsContainer that generates physics objects given the mesh and transform
                 *
                 * @param node
                 * @param mesh
                 * @param transform
                 */
                PhysicsContainer(
                        WS2Common::Scene::SceneNode *node,
                        WS2Common::Resource::ResourceMesh *mesh,
                        WS2Common::Transform &transform
                        );

                /**
                 * @brief Constructor for PhysicsContainer that generates physics objects given an AABB and transform
                 *
                 * @param aabb
                 * @param transform
                 *
                 */
                PhysicsContainer(WS2Common::AABB3 &aabb, WS2Common::Transform &transform);

                ~PhysicsContainer();

                void updateTransform(WS2Common::Transform &transform);

                btRigidBody* getRigidBody();
        };
    }
}

#endif

