/**
 * @file
 * @brief Header for the MeshCollisionSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_MESHCOLLISIONSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_MESHCOLLISIONSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/CollisionSceneNode.hpp"

namespace WS2Common {
    namespace Scene {

        /**
         * @brief Collision shape that uses every triangle of a mesh
         */
        class WS2COMMON_EXPORT MeshCollisionSceneNode : public CollisionSceneNode {
            protected:
                /**
                 * @brief The name of the referenced mesh
                 */
                QString meshName;
                /**
                 * @brief Flag applied to every collision triangle in the mesh (for bunkers in Golf, etc)
                 */
                quint16 collisionTriangleFlag = 0x00;

            protected:
                virtual void serializeNodeDataXml(QXmlStreamWriter &s) const;
                virtual const QString getSerializableName() const;

            public:
                MeshCollisionSceneNode() = default;
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

                /**
                 * @brief Getter for collisionTriangleFlag
                 *
                 * @param The collision triangle flag at 0x1E
                 */
                quint16 getCollisionTriangleFlag() const;

                /**
                 * @brief Setter for collisionTriangleFlag
                 *
                 * @param The collision triangle flag at 0x1E
                 */
                void setCollisionTriangleFlag(quint16 collisionTriangleFlag);
        };
    }
}

#endif

