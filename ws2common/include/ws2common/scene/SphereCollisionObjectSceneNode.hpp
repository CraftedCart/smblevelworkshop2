/**
 * @file
 * @brief Header for the SphereCollisionObjectSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SPHERECOLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SPHERECOLLISIONSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT SphereCollisionObjectSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;
                float radius;

            public:
                SphereCollisionObjectSceneNode() = default;
                SphereCollisionObjectSceneNode(const QString name);
                float getRadius() const;
                void setRadius(float radius);
        };
    }
}

#endif

