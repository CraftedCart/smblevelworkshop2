#include "ws2common/CollisionGrid.hpp"

namespace WS2Common {
    void CollisionGrid::setGridStart(const glm::vec2 gridStart) {
        this->gridStart = gridStart;
    }

    glm::vec2 CollisionGrid::getGridStart() {
        return gridStart;
    }

    void CollisionGrid::setGridStep(const glm::vec2 gridStep) {
        this->gridStep = gridStep;
    }

    glm::vec2 CollisionGrid::getGridStep() {
        return gridStep;
    }

    void CollisionGrid::setGridStepCount(const glm::uvec2 gridStepCount) {
        this->gridStepCount = gridStepCount;
    }

    glm::uvec2 CollisionGrid::getGridStepCount() {
        return gridStepCount;
    }
}

