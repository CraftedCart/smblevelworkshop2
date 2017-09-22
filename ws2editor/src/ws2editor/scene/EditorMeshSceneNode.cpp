#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/MathUtils.hpp"
#include <glm/gtc/quaternion.hpp>

namespace WS2Editor {
    namespace Scene {
        EditorMeshSceneNode::EditorMeshSceneNode(const QString name) : MeshSceneNode(name) {
            initPhysics();
        }

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, Resource::ResourceMesh *mesh) : MeshSceneNode(name) {
            this->mesh = mesh;
            initPhysics();
        }

        EditorMeshSceneNode::~EditorMeshSceneNode() {
            //mesh is not deleted as ResourceManager owns ResourceMeshs
            delete physicsCollisionShape;
            delete physicsMotionState;
            delete physicsRigidBody;
        }

        void EditorMeshSceneNode::initPhysics() {
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

            physicsRigidBody->setUserPointer(this); //The rigid body is bound to this EditorMeshSceneNode
        }

        const Resource::ResourceMesh* EditorMeshSceneNode::getMesh() const {
            return mesh;
        }

        btRigidBody* EditorMeshSceneNode::getPhysicsRigidBody() {
            return physicsRigidBody;
        }
    }
}

