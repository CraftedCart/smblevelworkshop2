/**
 * @file
 * @brief Header for the BumperSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BUMPERSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BUMPERSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT BumperSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;

            public:
                BumperSceneNode() = default;
                BumperSceneNode(const QString name);
        };
    }
}

#endif

