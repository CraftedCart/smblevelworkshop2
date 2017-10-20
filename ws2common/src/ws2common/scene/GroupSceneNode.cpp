#include "ws2common/scene/GroupSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        GroupSceneNode::GroupSceneNode(const QString name) : SceneNode(name) {}

        GroupSceneNode::~GroupSceneNode() {
            delete collisionGrid;
        }

        void GroupSceneNode::setCollisionGrid(CollisionGrid *collisionGrid) {
            this->collisionGrid = collisionGrid;
        }

        CollisionGrid* GroupSceneNode::getCollisionGrid() {
            return collisionGrid;
        }
    }
}

