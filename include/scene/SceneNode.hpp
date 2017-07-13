/**
 * @file
 * @brief Header for the WS2::Scene::SceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_SCENE_SCENENODE_HPP
#define SMBLEVELWORKSHOP2_SCENE_SCENENODE_HPP

#include <glm/glm.hpp>
#include <QVector>

namespace WS2 {
    namespace Scene {
        /**
         * @brief A node in a scene graph
         *
         * A node contains a transform, as well as various children which inherit the transform.
         */
        class SceneNode {
            protected:
                QVector<SceneNode*> children;

                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);

            public:
                /**
                 * @brief Deletes all children
                 */
                virtual ~SceneNode();

                QVector<SceneNode*>& getChildren();

                /**
                 * @return The position of the node relative to its parent
                 */
                glm::vec3& getPosition();

                /**
                 * @return The rotation of the node relative to its parent
                 */
                glm::vec3& getRotation();

                /**
                 * @return The position of the node relative to its parent
                 */
                glm::vec3& getScale();
        };
    }
}

#endif

