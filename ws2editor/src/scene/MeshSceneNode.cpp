#include "scene/MeshSceneNode.hpp"
#include "MathUtils.hpp"
#include <glm/gtc/quaternion.hpp>

namespace WS2Editor {
    namespace Scene {
        MeshSceneNode::MeshSceneNode(const QString name) : SceneNode(name) {
            initPhysics();
        }

        MeshSceneNode::MeshSceneNode(const QString name, Resource::ResourceMesh *mesh) : SceneNode(name) {
            this->mesh = mesh;
            initPhysics();
        }

        MeshSceneNode::~MeshSceneNode() {
            //mesh is not deleted as ResourceManager owns ResourceMeshs
            delete physicsCollisionShape;
            delete physicsMotionState;
            delete physicsRigidBody;
        }

        void MeshSceneNode::initPhysics() {
            glm::quat rotQuat = glm::quat(rotation);

            //Construct mesh collision shape
            btTriangleMesh *triMesh = new btTriangleMesh();

            const QVector<Model::MeshSegment*>& segments = mesh->getMeshSegments();

            //Loop over all segments in the mesh
            for (int i = 0; i < segments.size(); i++) {
                const Model::MeshSegment *segment = segments.at(i);

                for (int j = 0; j < segment->getIndices().size(); j += 3) {
                    triMesh->addTriangle(
                            MathUtils::toBtVector3(segment->getVertices().at(segment->getIndices().at(j)).position),
                            MathUtils::toBtVector3(segment->getVertices().at(segment->getIndices().at(j + 1)).position),
                            MathUtils::toBtVector3(segment->getVertices().at(segment->getIndices().at(j + 2)).position)
                            );
                }
            }

            physicsCollisionShape = new btBvhTriangleMeshShape(triMesh, true);

            physicsMotionState = new btDefaultMotionState(btTransform(
                        btQuaternion(rotQuat.x, rotQuat.y, rotQuat.z, rotQuat.w),
                        btVector3(position.x, position.y, position.z)
                        ));

            btRigidBody::btRigidBodyConstructionInfo constructionInfo(
                    0, //kg mass - 0 = static object
                    physicsMotionState,
                    physicsCollisionShape,
                    btVector3(0.0f, 0.0f, 0.0f) //Local inertia
                    );

            physicsRigidBody = new btRigidBody(constructionInfo);

            physicsRigidBody->setUserPointer(this); //The rigid body is bound to this MeshSceneNode
        }

        const Resource::ResourceMesh* MeshSceneNode::getMesh() const {
            return mesh;
        }

        btRigidBody* MeshSceneNode::getPhysicsRigidBody() {
            return physicsRigidBody;
        }
    }
}

