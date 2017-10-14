/**
 * @file
 * @brief Header for the GroupSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GROUPSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GROUPSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class GroupSceneNode : public SceneNode {
            public:
                GroupSceneNode(const QString name);
        };
    }
}

#endif

