/**
 * @file
 * @brief Header for the WormholeSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_WORMHOLESCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_WORMHOLESCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT WormholeSceneNode : public SceneNode {
            protected:
                /**
                 * @brief Where this wormhole should lead to
                 */
                WormholeSceneNode *destination;

            public:
                WormholeSceneNode(const QString name);

                /**
                 * @brief Getter for destination
                 *
                 * @return A pointer to the destination WormholeSceneNode
                 */
                WormholeSceneNode* getDestination();

                /**
                 * @brief Const etter for destination
                 *
                 * @return A pointer to the destination WormholeSceneNode
                 */
                const WormholeSceneNode* getDestination() const;

                /**
                 * @brief Setter for destination
                 *
                 * @param type A pointer to the destination wormhole to set this as
                 */
                void setDestination(WormholeSceneNode *destination);
        };
    }
}

#endif

