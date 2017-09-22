/**
 * @file
 * @brief Header for the PhysicsManager class
 */

#ifndef SMBLEVELWORKSHOP2_PHYSICS_PHYSICSMANAGER_HPP
#define SMBLEVELWORKSHOP2_PHYSICS_PHYSICSMANAGER_HPP

#include <btBulletDynamicsCommon.h>

namespace WS2Editor {
    namespace Physics {
        class PhysicsManager {
            protected:
                btBroadphaseInterface *broadphase;
                btDefaultCollisionConfiguration *collisionConfig;
                btCollisionDispatcher *collisionDispatcher;
                btSequentialImpulseConstraintSolver *solver;
                btDiscreteDynamicsWorld *dynamicsWorld;

            public:
                /**
                 * @brief Constructs a PhysicsManager and initializes bullet physics
                 */
                PhysicsManager();

                /**
                 * @brief Frees up resources
                 */
                ~PhysicsManager();

                /**
                 * @brief Getter for dynamicsWorld
                 *
                 * @return This PhysicsManager's dyanmicsWorld
                 */
                btDiscreteDynamicsWorld* getDynamicsWorld();

                /**
                 * @brief Adds a rigid body to dynamicsWorld
                 *
                 * @param rb The rigid body to add
                 */
                void addRigidBody(btRigidBody *rb);
        };
    }
}

#endif

