/**
 * @file
 * @brief Header for the ConeCollisionObjectSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_CONECOLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_CONECOLLISIONSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT ConeCollisionObjectSceneNode : public SceneNode {
            protected:
                virtual const QString getSerializableName() const;
                float radius, height;

            public:
                ConeCollisionObjectSceneNode() = default;
                ConeCollisionObjectSceneNode(const QString name);
                float getRadius() const;
                void setRadius(float radius);
                float getHeight() const;
                void setHeight(float height);
        };
    }
}

#endif

