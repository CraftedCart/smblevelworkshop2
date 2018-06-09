#include "propertiesproviderplugin/Plugin.hpp"
#include "propertiesproviderplugin/TransformWidget.hpp"
#include "propertiesproviderplugin/GoalWidget.hpp"
#include "propertiesproviderplugin/BananaWidget.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include <QVBoxLayout>
#include <QDebug>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor;
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Common::Scene;

        bool Plugin::init() {
            //Hook into the stage editor window by calling onStageEditorWindowConstructed after it's been created
            connect(WS2EditorInstance::getInstance(), &WS2EditorInstance::onStageEditorWindowConstructed,
                    this, &Plugin::onStageEditorWindowConstructed,
                    Qt::DirectConnection);

            //The plugin initialized successfully, return true
            qDebug() << "WS2Editor PropertiesProviderPlugin successfully initialized";
            return true;
        }

        void Plugin::onStageEditorWindowConstructed(UI::StageEditorWindow &w) {
            stageEditorWindow = &w;

            connect(w.getPropertiesWidget(), &PropertiesWidget::onUpdatePropertiesWidget,
                    this, &Plugin::onUpdatePropertiesWidget,
                    Qt::DirectConnection);
        }

        void Plugin::onUpdatePropertiesWidget(QVBoxLayout *layout, QVector<SceneNode*> &nodes) {
            if (nodes.isEmpty()) return;

            //Create widgets
            createTransformWidgets(layout, nodes);
            createGoalWidgets(layout, nodes);
            createBananaWidgets(layout, nodes);
        }

        void Plugin::createTransformWidgets(QVBoxLayout *layout, QVector<SceneNode*> &nodes) {
            TransformWidget *w = new TransformWidget(nodes, tr("Transform"));
            layout->addWidget(w);

            w->toggleContentShown(true); //Default to visible
        }

        void Plugin::createGoalWidgets(QVBoxLayout *layout, QVector<SceneNode*> &nodes) {
            //Fetch goals
            QVector<GoalSceneNode*> goals;
            for (SceneNode *node : nodes) {
                if (GoalSceneNode *goal = dynamic_cast<GoalSceneNode*>(node)) {
                    goals.append(goal);
                }
            }

            if (goals.size() == 0) return;

            GoalWidget *w = new GoalWidget(goals, tr("Goal"));
            layout->addWidget(w);

            w->toggleContentShown(true); //Default to visible
        }

        void Plugin::createBananaWidgets(QVBoxLayout *layout, QVector<SceneNode*> &nodes) {
            //Fetch goals
            QVector<BananaSceneNode*> bananas;
            for (SceneNode *node : nodes) {
                if (BananaSceneNode *banana = dynamic_cast<BananaSceneNode*>(node)) {
                    bananas.append(banana);
                }
            }

            if (bananas.size() == 0) return;

            BananaWidget *w = new BananaWidget(bananas, stageEditorWindow->getViewportWidget()->getRenderManager(), tr("Banana"));
            layout->addWidget(w);

            w->toggleContentShown(true); //Default to visible
        }

    }
}

