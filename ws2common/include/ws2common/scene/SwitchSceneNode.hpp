/**
 * @file
 * @brief Header for the SwitchSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SWITCHSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SWITCHSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/EnumPlaybackState.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT SwitchSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The type of this node
                 */
                EnumPlaybackState type;

                /**
                 * @brief The animation group that this switch affects
                 */
                unsigned short linkedAnimGroupId;

            public:
                SwitchSceneNode(const QString name);

                /**
                 * @brief Getter for type
                 *
                 * @return The type that this node is
                 */
                EnumPlaybackState getType() const;

                /**
                 * @brief Setter for type
                 *
                 * @param type The type to set this as
                 */
                void setType(EnumPlaybackState type);

                /**
                 * @brief Getter for linkedAnimGroupID
                 *
                 * @return The animation group ID that this switch affects
                 */
                unsigned short getLinkedAnimGroupId() const;

                /**
                 * @brief Setter for linkedAnimGroupID
                 *
                 * @param The animation group ID that this switch should affect
                 */
                void setLinkedAnimGroupId(unsigned short linkedAnimGroupId);
        };
    }
}

#endif

