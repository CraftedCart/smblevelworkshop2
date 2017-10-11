#include "ws2common/Stage.hpp"

namespace WS2Common {
    Stage::Stage() {
        rootNode = new Scene::SceneNode("root");
    }

    Stage::~Stage() {
        delete rootNode;
    }

    void Stage::setRootNode(Scene::SceneNode *rootNode) {
        this->rootNode = rootNode;
    }

    Scene::SceneNode* Stage::getRootNode() {
        return rootNode;
    }

    void Stage::setStartPos(const glm::vec3 startPos) {
        this->startPos = startPos;
    }

    glm::vec3 Stage::getStartPos() {
        return startPos;
    }

    void Stage::setFalloutY(const float falloutY) {
        this->falloutY = falloutY;
    }

    float Stage::getFalloutY() {
        return falloutY;
    }
}

