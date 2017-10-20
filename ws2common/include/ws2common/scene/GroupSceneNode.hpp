/**
 * @file
 * @brief Header for the GroupSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GROUPSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GROUPSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/CollisionGrid.hpp"

namespace WS2Common {
    namespace Scene {
        class GroupSceneNode : public SceneNode {
            protected:
                CollisionGrid *collisionGrid;

            public:
                GroupSceneNode(const QString name);
                ~GroupSceneNode();

                /**
                 * @brief Setter for collisionGrid
                 *
                 * @param collisionGrid The collision grid this GroupSceneNode should have
                 */
                void setCollisionGrid(CollisionGrid *collisionGrid);

                /**
                 * @brief Getter for collisionGrid
                 *
                 * @return collisionGrid The collision grid this GroupSceneNode has
                 */
                CollisionGrid* getCollisionGrid();
        };
    }
}

#endif

