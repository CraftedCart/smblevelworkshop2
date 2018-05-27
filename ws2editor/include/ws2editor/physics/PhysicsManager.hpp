/**
 * @file
 * @brief Header for the PhysicsManager class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PHYSICS_PHYSICSMANAGER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PHYSICS_PHYSICSMANAGER_HPP

#include "ws2editor_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include <btBulletDynamicsCommon.h>
#include <QHash>

namespace WS2Editor {
    namespace Physics {
        class WS2EDITOR_EXPORT PhysicsManager {
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

                /**
                 * @brief Removes a rigid body from dynamicsWorld
                 *
                 * @note rb is not deleted
                 *
                 * @param rb The rigid body to remove
                 */
                void removeRigidBody(btRigidBody *rb);

                //void addNode(SceneNode *node, )
        };
    }
}

#endif

