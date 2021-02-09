/**
 * @file
 * @brief Header for the BoosterSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BOOSTERSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BOOSTERSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT BoosterSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;

            public:
                BoosterSceneNode() = default;
                BoosterSceneNode(const QString name);
        };
    }
}

#endif

