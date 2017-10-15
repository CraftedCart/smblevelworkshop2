/**
 * @file
 * @brief Header for the SwitchSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SWITCHSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SWITCHSCENENODE_HPP

#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/EnumPlaybackState.hpp"

namespace WS2Common {
    namespace Scene {
        class SwitchSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The type of this node
                 */
                EnumPlaybackState type;

                /**
                 * @brief The animation group that this switch affects
                 */
                unsigned short animGroupId;

            public:
                SwitchSceneNode(const QString name);

                /**
                 * @brief Getter for type
                 *
                 * @return The type that this node is
                 */
                EnumPlaybackState getType();

                /**
                 * @brief Setter for type
                 *
                 * @param type The type to set this as
                 */
                void setType(EnumPlaybackState type);

                /**
                 * @brief Getter for animGroupID
                 *
                 * @return The animation group ID that this switch affects
                 */
                unsigned short getAnimGroupId();

                /**
                 * @brief Setter for animGroupID
                 *
                 * @param The animation group ID that this switch should affect
                 */
                void setAnimGroupId(unsigned short animGroupId);
        };
    }
}

#endif

