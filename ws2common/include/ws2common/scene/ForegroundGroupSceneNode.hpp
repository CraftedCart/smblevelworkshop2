/**
 * @file
 * @brief Header for the ForegroundGroupSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_FOREGROUNDGROUPSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_FOREGROUNDGROUPSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT ForegroundGroupSceneNode : public SceneNode {
            public:
                ForegroundGroupSceneNode(const QString name);
        };
    }
}

#endif

