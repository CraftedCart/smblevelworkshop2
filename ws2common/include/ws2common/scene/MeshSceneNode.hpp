/**
 * @file
 * @brief Header for the MeshSceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_MESHSCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_MESHSCENENODE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class WS2COMMON_EXPORT MeshSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The name of the referenced mesh
                 */
                QString meshName;

                /**
                 * @brief Whether the mesh should have runtime reflections of not
                 */
                bool runtimeReflective = false;

                /**
                 * @brief Bitflag for effects such as shadow casting/receiving and transparency
                 * TODO: Make this an actual bitflag instead of a uint handled by Blend2SMB or whatever
                 */
                unsigned int bitflag = 0;

                /**
                 * @brief Mesh 'type' for background and foreground objects. We don't actually know
                 * what the differences between these types are yet, though...
                 */

                unsigned int meshType = 0x1f;

            protected:
                virtual void serializeNodeDataXml(QXmlStreamWriter &s) const;
                virtual const QString getSerializableName() const;

            public:
                MeshSceneNode() = default;
                MeshSceneNode(const QString name);

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
                 * @brief Setter for runtimeReflective
                 *
                 * @param runtimeReflective Whether this mesh should be reflective (Using runtime generated reflections)
                 */
                void setRuntimeReflective(bool runtimeReflective);

                /**
                 * @brief Getter for runtimeReflective
                 *
                 * @return Whether this mesh is be reflective or not (Using runtime generated reflections)
                 */
                bool isRuntimeReflective() const;

                void setBitflag(unsigned int bitflag);
                unsigned int getBitflag() const;
                void setMeshType(unsigned int meshType);
                unsigned int getMeshType() const;
        };
    }
}

#endif

