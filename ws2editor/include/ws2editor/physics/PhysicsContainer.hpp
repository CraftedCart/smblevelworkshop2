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
        using namespace WS2Common;
        using namespace WS2Common::Resource;
        using namespace WS2Common::Scene;

        class WS2EDITOR_EXPORT PhysicsContainer {
            protected:
                btCollisionShape *physicsCollisionShape;
                btDefaultMotionState *physicsMotionState;
                btRigidBody *physicsRigidBody;

            public:
                /**
                 * @brief Constructor for PhysicsContainer that generates physics objects given the mesh and transform
                 *
                 * @param mesh
                 */
                PhysicsContainer(SceneNode *node, ResourceMesh *mesh, Transform &transform);
                ~PhysicsContainer();

                void updateTransform(Transform &transform);

                btRigidBody* getRigidBody();
        };
    }
}

#endif

