/**
 * @file
 * @brief Header for the GoalSceneNode class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GOALSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_GOALSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/EnumGoalType.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT GoalSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The type that this node is
                 */
                EnumGoalType type = EnumGoalType::BLUE;

            protected:
                virtual void serializeNodeDataXml(QXmlStreamWriter &s) const;
                virtual const QString getSerializableName() const;

            public:
                GoalSceneNode() = default;
                GoalSceneNode(const QString name);

                /**
                 * @brief Getter for type
                 *
                 * @return The type that this node is
                 */
                EnumGoalType getType() const;

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

