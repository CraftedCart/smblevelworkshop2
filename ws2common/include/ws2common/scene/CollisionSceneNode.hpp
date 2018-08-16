/**
 * @file
 * @brief Header for the CollisionSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_COLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_COLLISIONSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {

        /**
         * @brief Base class for all collision shapes
         */
        class WS2COMMON_EXPORT CollisionSceneNode : public SceneNode {
            protected:
                virtual void serializeNodeDataXml(QXmlStreamWriter &s) const;
                virtual const QString getSerializableName() const;

            public:
                CollisionSceneNode() = default;
                CollisionSceneNode(const QString name);
        };
    }
}

#endif

