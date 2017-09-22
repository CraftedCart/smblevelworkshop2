/**
 * @file
 * @brief Header for the BumperSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BUMPERSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BUMPERSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class BumperSceneNode : public SceneNode {
            public:
                BumperSceneNode(const QString name);
        };
    }
}

#endif

