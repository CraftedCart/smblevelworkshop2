#include "ws2common/Stage.hpp"
#include "ws2common/WS2Common.hpp"
#include <QCoreApplication>

namespace WS2Common {
    Stage::Stage() {
        rootNode = new Scene::SceneNode("root");
        rootNode->addChild(new Scene::BackgroundGroupSceneNode(QCoreApplication::translate("Stage", "Background Group")));
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

    const Scene::SceneNode* Stage::getRootNode() const {
        return rootNode;
    }

    void Stage::setFalloutY(const float falloutY) {
        this->falloutY = falloutY;
    }

    float Stage::getFalloutY() const {
        return falloutY;
    }

    Scene::BackgroundGroupSceneNode* Stage::getFirstBackgroundGroup(bool createIfNonExistent) {
        foreach(Scene::SceneNode *node, rootNode->getChildren()) {
            if(instanceOf<Scene::BackgroundGroupSceneNode>(node)) return static_cast<Scene::BackgroundGroupSceneNode*>(node);
        }

        //No background nodes found
        if (createIfNonExistent) {
            Scene::BackgroundGroupSceneNode *node = new Scene::BackgroundGroupSceneNode(QCoreApplication::translate("Stage", "Background Group"));
            rootNode->addChild(node);
            return node;
        } else {
            return nullptr;
        }
    }
}

