/**
 * @file
 * @brief Header for the CylinderCollisionObjectSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_CYLINDERCOLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_CYLINDERCOLLISIONSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT CylinderCollisionObjectSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;
                float radius, height;

            public:
                CylinderCollisionObjectSceneNode() = default;
                CylinderCollisionObjectSceneNode(const QString name);
                float getRadius() const;
                void setRadius(float radius);
                float getHeight() const;
                void setHeight(float height);
        };
    }
}

#endif

