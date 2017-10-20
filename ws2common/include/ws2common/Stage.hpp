/**
 * @file
 * @brief Header for the Stage class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_STAGE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_STAGE_HPP

#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/scene/BackgroundGroupSceneNode.hpp"
#include <glm/glm.hpp>

namespace WS2Common {
    class Stage {
        protected:
            Scene::SceneNode *rootNode;

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
             * @brief Const getter for rootNode
             *
             * @return A pointer to the root scene node of the stage
             */
            const Scene::SceneNode* getRootNode() const;

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
            float getFalloutY() const;

            /**
             * @brief Finds the first background group scene node, by searching direct children of rootNode
             *
             * @param createIfNonExistent Should a new background group be created and returned if none are found?
             *
             * @return The first background group scene node, or nullptr if there are none and createIfNonExistent is false
             */
            Scene::BackgroundGroupSceneNode* getFirstBackgroundGroup(bool createIfNonExistent = false);
    };
}

#endif

