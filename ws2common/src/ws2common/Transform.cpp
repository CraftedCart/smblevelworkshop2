#include "ws2common/Transform.hpp"

namespace WS2Common {
    glm::vec3 Transform::getPosition() const {
        return position;
    }

    void Transform::setPosition(const glm::vec3 position) {
        this->position = position;
    }

    glm::vec3 Transform::getRotation() const {
        return rotation;
    }

    void Transform::setRotation(const glm::vec3 rotation) {
        this->rotation = rotation;
    }

    glm::vec3 Transform::getScale() const {
        return scale;
    }

    void Transform::setScale(const glm::vec3 scale) {
        this->scale = scale;
    }
}

