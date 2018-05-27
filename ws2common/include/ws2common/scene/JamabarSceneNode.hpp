/**
 * @file
 * @brief Header for the JamabarSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_JAMABARSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_JAMABARSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT JamabarSceneNode : public SceneNode {
            public:
                JamabarSceneNode(const QString name);
        };
    }
}

#endif

