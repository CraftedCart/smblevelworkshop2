#include "ws2common/Stage.hpp"
#include "ws2common/WS2Common.hpp"
#include <QCoreApplication>

namespace WS2Common {
    Stage::Stage() {
        rootNode = new Scene::SceneNode("root");
        rootNode->addChild(new Scene::BackgroundGroupSceneNode(QCoreApplication::translate("Stage", "Background Group")));
        rootNode->addChild(new Scene::ForegroundGroupSceneNode(QCoreApplication::translate("Stage", "Foreground Group")));
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

    void Stage::addModel(const QUrl url) {
        models.append(url);
    }

    QVector<QUrl> Stage::getModels() {
        return models;
    }

    const QVector<QUrl> Stage::getModels() const {
        return models;
    }

    Scene::BackgroundGroupSceneNode* Stage::getFirstBackgroundGroup(bool createIfNonExistent) {
        foreach(Scene::SceneNode *node, rootNode->getChildren()) {
            //TODO: Replace this with some dynamic cast thingy
            if (Scene::BackgroundGroupSceneNode *bg = dynamic_cast<Scene::BackgroundGroupSceneNode*>(node)) return bg;
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

    Scene::ForegroundGroupSceneNode* Stage::getFirstForegroundGroup(bool createIfNonExistent) {
        foreach(Scene::SceneNode *node, rootNode->getChildren()) {
            //TODO: Replace this with some dynamic cast thingy
            if (Scene::ForegroundGroupSceneNode *fg = dynamic_cast<Scene::ForegroundGroupSceneNode*>(node)) return fg;
        }

        //No foreground nodes found
        if (createIfNonExistent) {
            Scene::ForegroundGroupSceneNode *node = new Scene::ForegroundGroupSceneNode(QCoreApplication::translate("Stage", "Foreground Group"));
            rootNode->addChild(node);
            return node;
        } else {
            return nullptr;
        }
    }
}

