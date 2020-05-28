#include "ws2common/scene/ConeCollisionObjectSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        ConeCollisionObjectSceneNode::ConeCollisionObjectSceneNode(const QString name) : SceneNode(name) {}

        const QString ConeCollisionObjectSceneNode::getSerializableName() const {
            return "coneCollisionObjectSceneNode";
        }

        float ConeCollisionObjectSceneNode::getRadius() const {
            return radius;
        }

        void ConeCollisionObjectSceneNode::setRadius(float radius) {
            this->radius = radius;
        }

        float ConeCollisionObjectSceneNode::getHeight() const {
            return height;
        }

        void ConeCollisionObjectSceneNode::setHeight(float height) {
            this->height = height;
        }

    }
}

