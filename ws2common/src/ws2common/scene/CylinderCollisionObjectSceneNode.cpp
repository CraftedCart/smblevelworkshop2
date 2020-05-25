#include "ws2common/scene/CylinderCollisionObjectSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        CylinderCollisionObjectSceneNode::CylinderCollisionObjectSceneNode(const QString name) : SceneNode(name) {}

        const QString CylinderCollisionObjectSceneNode::getSerializableName() const {
            return "cylinderCollisionObjectSceneNode";
        }

        float CylinderCollisionObjectSceneNode::getRadius() const {
            return radius;
        }

        void CylinderCollisionObjectSceneNode::setRadius(float radius) {
            this->radius = radius;
        }

        float CylinderCollisionObjectSceneNode::getHeight() const {
            return height;
        }

        void CylinderCollisionObjectSceneNode::setHeight(float height) {
            this->height = height;
        }
    }
}

