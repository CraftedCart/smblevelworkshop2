/**
 * @file
 * @brief Header for the GoalSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GOALSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GOALSCENENODE_HPP

#include "scene/SceneNode.hpp"
#include "EnumGoalType.hpp"

namespace WS2Common {
    namespace Scene {
        class GoalSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The type that this node is
                 */
                EnumGoalType type;

            public:
                GoalSceneNode(const QString name);

                /**
                 * @brief Getter for type
                 *
                 * @return The type that this node is
                 */
                EnumGoalType getType();

                /**
                 * @brief Setter for type
                 *
                 * @param type The type to set this as
                 */
                void setType(EnumGoalType type);
        };
    }
}

#endif

