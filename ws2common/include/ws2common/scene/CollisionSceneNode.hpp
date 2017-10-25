/**
 * @file
 * @brief Header for the CollisionSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_COLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_COLLISIONSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {

        /**
         * @brief Near base class for all collision shapes
         */
        class CollisionSceneNode : public SceneNode {
            public:
                CollisionSceneNode(const QString name);
        };
    }
}

#endif

