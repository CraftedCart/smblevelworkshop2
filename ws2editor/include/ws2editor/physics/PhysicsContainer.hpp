/**
 * @file
 * @brief Header for the PhysicsContainer class
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
                 * @brief Constructor for PhysicsContainer that generates physics objects given the meshes and transform
                 *
                 * @param node
                 * @param meshes
                 * @param transform
                 */
                PhysicsContainer(
                        WS2Common::Scene::SceneNode *node,
                        const QVector<WS2Common::Resource::ResourceMesh*> meshes,
                        const WS2Common::Transform &transform
                        );

                /**
                 * @brief Constructor for PhysicsContainer that generates physics objects given the mesh and transform
                 *
                 * @param node
                 * @param mesh
                 * @param transform
                 */
                PhysicsContainer(
                        WS2Common::Scene::SceneNode *node,
                        WS2Common::Resource::ResourceMesh* mesh,
                        const WS2Common::Transform &transform
                        );

                ~PhysicsContainer();

                void updateTransform(WS2Common::Transform &transform);

                btRigidBody* getRigidBody();
        };
    }
}

#endif

