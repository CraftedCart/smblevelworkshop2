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

        void SceneNode::addChild(SceneNode *child) {
            children.append(child);
        }

        glm::vec3 SceneNode::getPosition() const {
            return position;
        }

        glm::vec3 SceneNode::getRotation() const {
            return rotation;
        }

        glm::vec3 SceneNode::getScale() const {
            return scale;
        }
    }
}

