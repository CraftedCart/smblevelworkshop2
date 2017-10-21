#include "ws2common/CollisionGrid.hpp"

namespace WS2Common {
    void CollisionGrid::setGridStart(const glm::vec2 gridStart) {
        this->gridStart = gridStart;
    }

    const glm::vec2 CollisionGrid::getGridStart() const {
        return gridStart;
    }

    void CollisionGrid::setGridStep(const glm::vec2 gridStep) {
        this->gridStep = gridStep;
    }

    const glm::vec2 CollisionGrid::getGridStep() const {
        return gridStep;
    }

    void CollisionGrid::setGridStepCount(const glm::uvec2 gridStepCount) {
        this->gridStepCount = gridStepCount;
    }

    const glm::uvec2 CollisionGrid::getGridStepCount() const {
        return gridStepCount;
    }
}

