/**
 * @file
 * @brief Header for the MeshSceneNode class
 */

#include "scene/SceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        class MeshSceneNode : public SceneNode {
            protected:
                /**
                 * @brief The name of the referenced mesh
                 */
                QString meshName;

            public:
                MeshSceneNode(QString name);

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
                QString getMeshName();
        };
    }
}

