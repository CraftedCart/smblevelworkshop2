/**
 * @file
 * @brief Header for the Stage class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_STAGE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_STAGE_HPP

#include "ws2common_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/scene/BackgroundGroupSceneNode.hpp"
#include "ws2common/scene/ForegroundGroupSceneNode.hpp"
#include "ws2common/EnumStageType.hpp"
#include "ws2common/Fog.hpp"
#include "ws2common/animation/FogAnimation.hpp"
#include <glm/glm.hpp>
#include <QVector>
#include <QUrl>

namespace WS2Common {
    class WS2COMMON_EXPORT Stage {
        protected:
            Scene::SceneNode *rootNode;

            EnumStageType stageType = EnumStageType::MAIN_GAME;
            float falloutY;
            Fog* fog = nullptr;
            Animation::FogAnimation* fogAnimation = nullptr;

            /**
             * @brief All 3D model files that this stage may or may not use
             */
            QVector<QUrl> models;

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
             * @brief Adds an entry to the models vector
             *
             * @param url The model entry to add
             */
            void addModel(const QUrl url);

            /**
             * @brief Getter for models
             *
             * @return Model files that this stage may use
             */
            QVector<QUrl> getModels();

            /**
             * @brief Const getter for models
             *
             * @return Model files that this stage may use
             */
            const QVector<QUrl> getModels() const;

            /**
             * @brief Finds the first background group scene node, by searching direct children of rootNode
             *
             * @param createIfNonExistent Should a new background group be created and returned if none are found?
             *
             * @return The first background group scene node, or nullptr if there are none and createIfNonExistent is false
             */
            Scene::BackgroundGroupSceneNode* getFirstBackgroundGroup(bool createIfNonExistent = false);
            Scene::ForegroundGroupSceneNode* getFirstForegroundGroup(bool createIfNonExistent = false);

            /**
             * @brief Getter for stageType
             * @return
             */
            EnumStageType getStageType() const;

            /**
             * @brief Setter for stageType
             * @param value
             */
            void setStageType(const EnumStageType &value);

            /**
             * @brief Getter for fog
             * @return
             */
            Fog *getFog() const;

            /**
             * @brief Setter for fog
             * @param value
             */
            void setFog(Fog *value);

            /**
             * @brief Getter for fog animation
             * @return
             */
            Animation::FogAnimation* getFogAnimation() const;

            /**
             * @brief Setter for fog animation
             * @param value
             */
            void setFogAnimation(Animation::FogAnimation *value);
    };
}

#endif

