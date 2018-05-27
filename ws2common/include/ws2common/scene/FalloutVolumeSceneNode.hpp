/**
 * @file
 * @brief Header for the FalloutVolumeSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_FALLOUTVOLUMESCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_FALLOUTVOLUMESCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT FalloutVolumeSceneNode : public SceneNode {
            public:
                FalloutVolumeSceneNode(const QString name);
        };
    }
}

#endif

