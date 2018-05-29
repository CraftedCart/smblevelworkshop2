#include "rendercolorsplugin/Plugin.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include "ws2editor/rendering/MeshRenderCommand.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include <QDebug>

namespace WS2EditorPlugins {
    namespace RenderColorsPlugin {
        using namespace WS2Editor;
        using namespace WS2Common;
        using namespace WS2Common::Scene;
        using namespace WS2Common::Resource;
        using namespace WS2Common::Model;

        bool Plugin::init() {
            //Hook into the stage editor window by calling onStageEditorWindowConstructed after it's been created
            connect(WS2EditorInstance::getInstance(), &WS2EditorInstance::onStageEditorWindowConstructed,
                    this, &Plugin::onStageEditorWindowConstructed,
                    Qt::DirectConnection);

            //The plugin initialized successfully, return true
            qDebug() << "WS2Editor RenderColorsPlugin successfully initialized";
            return true;
        }

        void Plugin::onStageEditorWindowConstructed(UI::StageEditorWindow &w) {
            connect(w.getViewportWidget()->getRenderManager(), &RenderManager::postEnqueueSceneNodeRenderMesh,
                    this, &Plugin::onPostEnqueueSceneNodeRenderMesh,
                    Qt::DirectConnection);
        }

        void Plugin::onPostEnqueueSceneNodeRenderMesh(
                WS2Common::Scene::SceneNode *node,
                WS2Editor::Rendering::MeshRenderCommand *command
                ) {
            if (const GoalSceneNode *goal = dynamic_cast<const GoalSceneNode*>(node)) {
                switch (goal->getType()) {
                    case EnumGoalType::BLUE:
                        command->setTint(glm::vec4(0.13f, 0.59f, 0.95f, 1.0f)); break;
                    case EnumGoalType::GREEN:
                        command->setTint(glm::vec4(0.30f, 0.69f, 0.51f, 1.0f)); break;
                    case EnumGoalType::RED:
                        command->setTint(glm::vec4(0.95f, 0.26f, 0.21f, 1.0f)); break;
                }
            } else if (const BumperSceneNode *bumper = dynamic_cast<const BumperSceneNode*>(node)) {
                command->setTint(glm::vec4(1.0f, 0.6f, 0.0f, 1.0f));
            } else if (const BananaSceneNode *bumper = dynamic_cast<const BananaSceneNode*>(node)) {
                command->setTint(glm::vec4(1.0f, 0.92f, 0.23f, 1.0f));
            }
        }

    }
}

