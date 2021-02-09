/**
 * @file
 * @brief Header for the GolfHoleSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GOLFHOLESCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GOLFHOLESCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT GolfHoleSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;

            public:
                GolfHoleSceneNode() = default;
                GolfHoleSceneNode(const QString name);
        };
    }
}

#endif

