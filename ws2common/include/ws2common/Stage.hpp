/**
 * @file
 * @brief Header for the Stage class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_STAGE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_STAGE_HPP

#include "ws2common/scene/SceneNode.hpp"
#include "glm/glm.hpp"

namespace WS2Common {
    class Stage {
        protected:
            Scene::SceneNode *rootNode;

            glm::vec3 startPos;
            float falloutY;
            //TODO: Fog
            //TODO: Animated fog

        public:
            /**
             * @brief Constructor for Stage
             *
             * Creates an empty stage with a root node
             */
            Stage();

            /**
             * @brief Destructor for Stage - Performs cleanup
             */
            ~Stage();

            /**
             * @brief Setter for rootNode
             *
             * @param rootNode A pointer to the root scene node of the stage to set
             */
            void setRootNode(Scene::SceneNode *rootNode);

            /**
             * @brief Getter for rootNode
             *
             * @return A pointer to the root scene node of the stage
             */
            Scene::SceneNode* getRootNode();

            /**
             * @brief Setter for startPos
             *
             * @param startPos The starting position of the stage to set
             */
            void setStartPos(const glm::vec3 startPos);

            /**
             * @brief Getter for startPos
             *
             * @return The starting position of the stage
             */
            glm::vec3 getStartPos();

            /**
             * @brief Setter for falloutY
             *
             * @param falloutY The fallout Y position to set
             */
            void setFalloutY(const float falloutY);

            /**
             * @brief Getter for falloutY
             *
             * @return The fallout Y position to get
             */
            float getFalloutY();
    };
}

#endif

