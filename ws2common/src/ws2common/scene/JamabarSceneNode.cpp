#include "ws2common/scene/JamabarSceneNode.hpp"

namespace WS2Common {
    namespace Scene {
        JamabarSceneNode::JamabarSceneNode(const QString name) : SceneNode(name) {}

        const QString JamabarSceneNode::getSerializableName() const {
            return "jamabarSceneNode";
        }
    }
}

