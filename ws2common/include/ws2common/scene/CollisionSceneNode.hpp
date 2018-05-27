/**
 * @file
 * @brief Header for the CollisionSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_COLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_COLLISIONSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {

        /**
         * @brief Near base class WS2COMMON_EXPORT for all collision shapes
         */
        class WS2COMMON_EXPORT CollisionSceneNode : public SceneNode {
            public:
                CollisionSceneNode(const QString name);
        };
    }
}

#endif

