/**
 * @file
 * @brief Header for the Transform class WS2COMMON_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_TRANSFORM_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_TRANSFORM_HPP

#include "ws2common_export.h"
#include <glm/glm.hpp>

namespace WS2Common {
    class WS2COMMON_EXPORT Transform {
        protected:
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

        public:
            /**
             * @brief Getter for position
             *
             * @return The position of the transform
             */
            glm::vec3 getPosition() const;

            /**
             * @brief Setter for position
             *
             * @param position The new position to set
             */
            void setPosition(const glm::vec3 position);

            /**
             * @brief Offsets the position by the delta
             *
             * @param delta The amount to offset position by
             */
            void translate(const glm::vec3 delta);

            /**
             * @brief Getter for rotation
             *
             * @return The rotation of the transform
             */
            glm::vec3 getRotation() const;

            /**
             * @brief Setter for rotation
             *
             * @param rotation The new rotation to set
             */
            void setRotation(const glm::vec3 rotation);

            /**
             * @brief Getter for scale
             *
             * @return The position of the transform
             */
            glm::vec3 getScale() const;

            /**
             * @brief Setter for scale
             *
             * @param scale The new scale to set
             */
            void setScale(const glm::vec3 scale);
    };
}

#endif

