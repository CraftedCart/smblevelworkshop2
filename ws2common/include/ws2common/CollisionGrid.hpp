/**
 * @file
 * @brief Header for the Collision Grid class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_COLLISIONGRID_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_COLLISIONGRID_HPP

#include <QXmlStreamWriter>
#include <glm/glm.hpp>

namespace WS2Common {
    class CollisionGrid {
        protected:
            glm::vec2 gridStart = glm::vec2(-256.0f, -256.0f);
            glm::vec2 gridStep = glm::vec2(32.0f, 32.0f);
            glm::uvec2 gridStepCount = glm::uvec2(16, 16);

        public:
            /**
             * @brief Setter for gridStart
             *
             * @param gridStart The position where the collision grid should start
             */
            void setGridStart(const glm::vec2 gridStart);

            /**
             * @brief Getter for gridStart
             *
             * @return The position where the collision grid starts
             */
            const glm::vec2 getGridStart() const;

            /**
             * @brief Setter for gridStep
             *
             * @param gridStep How large each grid tile should be
             */
            void setGridStep(const glm::vec2 gridStep);

            /**
             * @brief Getter for gridStep
             *
             * @return How large each grid tile is
             */
            const glm::vec2 getGridStep() const;

            /**
             * @brief Setter for gridStepCount
             *
             * @param gridStepCount How many grid tiles there should be in the X x Y direction
             */
            void setGridStepCount(const glm::uvec2 gridStepCount);

            /**
             * @brief Getter for gridStepCount
             *
             * @return How many grid tiles there are in the X x Y direction
             */
            const glm::uvec2 getGridStepCount() const;

            void serializeDataXml(QXmlStreamWriter &s) const;
            static CollisionGrid* deserializeDataXml(QXmlStreamReader &xml);
    };
}

#endif

