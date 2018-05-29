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
                 * @brief The UUID of the wormhole where this wormhole should lead to
                 */
                QUuid destinationUuid = QUuid();

            public:
                WormholeSceneNode(const QString name);

                /**
                 * @brief Const getter for destinationUuid
                 *
                 * @return A pointer to the destination WormholeSceneNode
                 */
                const QUuid& getDestinationUuid() const;

                /**
                 * @brief Setter for destinationUuid
                 *
                 * @param type The UUID of the destination wormhole node to set this as
                 */
                void setDestinationUuid(QUuid destinationUuid);
        };
    }
}

#endif

