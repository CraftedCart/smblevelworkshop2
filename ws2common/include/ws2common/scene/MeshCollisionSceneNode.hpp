/**
 * @file
 * @brief Header for the MeshCollisionSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_MESHCOLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_MESHCOLLISIONSCENENODE_HPP

#include "ws2common/scene/CollisionSceneNode.hpp"

namespace WS2Common {
    namespace Scene {

        /**
         * @brief Collision shape that uses every triangle of a mesh
         */
        class MeshCollisionSceneNode : public CollisionSceneNode {
            protected:
                /**
                 * @brief The name of the referenced mesh
                 */
                QString meshName;

            public:
                MeshCollisionSceneNode(const QString name);

                /**
                 * @brief Setter for meshName
                 *
                 * @param meshName The new name of the referenced mesh to set
                 */
                void setMeshName(QString meshName);

                /**
                 * @brief Getter for meshName
                 *
                 * @return The name of the referenced mesh
                 */
                const QString getMeshName() const;
        };
    }
}

#endif

