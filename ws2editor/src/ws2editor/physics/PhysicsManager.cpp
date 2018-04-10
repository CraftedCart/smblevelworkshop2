#include "ws2editor/physics/PhysicsManger.hpp"

namespace WS2Editor {
    namespace Physics {
        PhysicsManager::PhysicsManager() {
            broadphase = new btDbvtBroadphase();
            collisionConfig = new btDefaultCollisionConfiguration();
            collisionDispatcher = new btCollisionDispatcher(collisionConfig);
            solver = new btSequentialImpulseConstraintSolver();
            dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphase, solver, collisionConfig);
            dynamicsWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
        }

        PhysicsManager::~PhysicsManager() {
            delete broadphase;
            delete collisionConfig;
            delete collisionDispatcher;
            delete solver;
            delete dynamicsWorld;
        }

        btDiscreteDynamicsWorld* PhysicsManager::getDynamicsWorld() {
            return dynamicsWorld;
        }

        void PhysicsManager::addRigidBody(btRigidBody *rb) {
            dynamicsWorld->addRigidBody(rb);
        }

        void PhysicsManager::removeRigidBody(btRigidBody *rb) {
            dynamicsWorld->removeRigidBody(rb);
        }
    }
}

