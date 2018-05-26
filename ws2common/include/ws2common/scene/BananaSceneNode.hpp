/**
 * @file
 * @brief Header for the BananaSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BANANASCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_BANANASCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/EnumBananaType.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT BananaSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The type of this node
                 */
                EnumBananaType type;

            public:
                BananaSceneNode(const QString name);

                /**
                 * @brief Getter for type
                 *
                 * @return The type that this node is
                 */
                EnumBananaType getType() const;

                /**
                 * @brief Setter for type
                 *
                 * @param type The type to set this as
                 */
                void setType(EnumBananaType type);
        };
    }
}

#endif

