#include "ws2editor/scene/EditorMeshSceneNode.hpp"
#include "ws2editor/resource/ResourceScene.hpp"
#include "ws2editor/MathUtils.hpp"
#include <glm/gtc/quaternion.hpp>

namespace WS2Editor {
    namespace Scene {
        using namespace WS2Editor::Resource;
        using namespace WS2Common::Resource;

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, ResourceScene *scene) :
            MeshSceneNode(name),
            scene(scene) {
            initPhysics();
        }

        EditorMeshSceneNode::EditorMeshSceneNode(const QString name, ResourceScene *scene, WS2Common::Resource::ResourceMesh *mesh) :
            MeshSceneNode(name),
            scene(scene) {
            this->mesh = mesh;
            initPhysics();
        }

        EditorMeshSceneNode::~EditorMeshSceneNode() {
            //mesh is not deleted as ResourceManager owns ResourceMeshs
            scene->getPhysicsManager()->removeRigidBody(physicsRigidBody);

            delete physicsCollisionShape;
            delete physicsMotionState;
            delete physicsRigidBody;
        }

        void EditorMeshSceneNode::initPhysics() {
            glm::quat rotQuat = glm::quat(transform.getRotation());

            //Construct mesh collision shape
            btTriangleMesh *triMesh = new btTriangleMesh();

            const QVector<WS2Common::Model::MeshSegment*>& segments = mesh->getMeshSegments();

            //Loop over all segments in the mesh
            for (int i = 0; i < segments.size(); i++) {
                const WS2Common::Model::MeshSegment *segment = segments.at(i);

                for (int j = 0; j < segment->getIndices().size(); j += 3) {
                    triMesh->addTriangle(
                            WS2Common::MathUtils::toBtVector3(segment->getVertices().at(segment->getIndices().at(j)).position),
                            WS2Common::MathUtils::toBtVector3(segment->getVertices().at(segment->getIndices().at(j + 1)).position),
                            WS2Common::MathUtils::toBtVector3(segment->getVertices().at(segment->getIndices().at(j + 2)).position)
                            );
                }
            }

            physicsCollisionShape = new btBvhTriangleMeshShape(triMesh, true);

            physicsMotionState = new btDefaultMotionState(btTransform(
                        btQuaternion(rotQuat.x, rotQuat.y, rotQuat.z, rotQuat.w),
                        btVector3(transform.getPosition().x, transform.getPosition().y, transform.getPosition().z)
                        ));

            btRigidBody::btRigidBodyConstructionInfo constructionInfo(
                    0, //kg mass - 0 = static object
                    physicsMotionState,
                    physicsCollisionShape,
                    btVector3(0.0f, 0.0f, 0.0f) //Local inertia
                    );

            physicsRigidBody = new btRigidBody(constructionInfo);

            physicsRigidBody->setUserPointer(this); //The rigid body is bound to this EditorMeshSceneNode

            //Register the physics object
            scene->getPhysicsManager()->addRigidBody(physicsRigidBody);
        }

        const WS2Common::Resource::ResourceMesh* EditorMeshSceneNode::getMesh() const {
            return mesh;
        }

        btRigidBody* EditorMeshSceneNode::getPhysicsRigidBody() {
            return physicsRigidBody;
        }
    }
}

