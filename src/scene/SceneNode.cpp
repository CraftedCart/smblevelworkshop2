#include "scene/SceneNode.hpp"
#include "ui/ModelManager.hpp"
#include <QtAlgorithms>

namespace WS2 {
    namespace Scene {
        SceneNode::~SceneNode() {
            qDeleteAll(children);
        }

        QVector<SceneNode*>& SceneNode::getChildren() {
            return children;
        }

        SceneNode* SceneNode::getChildByIndex(int index) {
            return children[index];
        }

        int SceneNode::getIndex() {
            if (parent) {
                return parent->getChildren().indexOf(const_cast<SceneNode*>(this));
            } else {
                return 0;
            }
        }

        void SceneNode::addChild(SceneNode *child) {
            child->setParent(this);
            children.append(child);
            UI::ModelManager::modelOutliner->onNodeAdded(child);
        }

        int SceneNode::getChildCount() {
            return children.size();
        }

        SceneNode* SceneNode::getParent() {
            return parent;
        }

        void SceneNode::setParent(SceneNode *parent) {
            this->parent = parent;
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

