#include "ws2common/scene/SphereCollisionObjectSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        SphereCollisionObjectSceneNode::SphereCollisionObjectSceneNode(const QString name) : SceneNode(name) {}

        const QString SphereCollisionObjectSceneNode::getSerializableName() const {
            return "sphereCollisionObjectSceneNode";
        }

        float SphereCollisionObjectSceneNode::getRadius() const {
            return radius;
        }

        void SphereCollisionObjectSceneNode::setRadius(float radius) {
            this->radius = radius;
        }
    }
}

