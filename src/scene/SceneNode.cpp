#include "scene/SceneNode.hpp"
#include <QtAlgorithms>

namespace WS2 {
    namespace Scene {
        SceneNode::~SceneNode() {
            qDeleteAll(children);
        }

        QVector<SceneNode*>& SceneNode::getChildren() {
            return children;
        }

        glm::vec3& SceneNode::getPosition() {
            return position;
        }

        glm::vec3& SceneNode::getRotation() {
            return position;
        }

        glm::vec3& SceneNode::getScale() {
            return position;
        }
    }
}

