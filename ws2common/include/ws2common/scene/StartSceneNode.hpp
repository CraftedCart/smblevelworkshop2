/**
 * @file
 * @brief Header for the StartSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_STARTSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_STARTSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class StartSceneNode : public SceneNode {
            public:
                StartSceneNode(const QString name);
        };
    }
}

#endif

