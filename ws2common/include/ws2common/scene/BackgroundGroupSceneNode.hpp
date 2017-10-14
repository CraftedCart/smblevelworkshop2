/**
 * @file
 * @brief Header for the BackgroundGroupSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BACKGROUNDGROUPSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BACKGROUNDGROUPSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class BackgroundGroupSceneNode : public SceneNode {
            public:
                BackgroundGroupSceneNode(const QString name);
        };
    }
}

#endif

