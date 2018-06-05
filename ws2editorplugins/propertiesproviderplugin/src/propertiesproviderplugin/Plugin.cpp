#include "propertiesproviderplugin/Plugin.hpp"
#include "propertiesproviderplugin/TransformWidget.hpp"
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
            connect(w.getPropertiesWidget(), &PropertiesWidget::onUpdatePropertiesWidget,
                    this, &Plugin::onUpdatePropertiesWidget,
                    Qt::DirectConnection);
        }

        void Plugin::onUpdatePropertiesWidget(QVBoxLayout *layout, QVector<SceneNode*> &nodes) {
            if (nodes.isEmpty()) return;

            //Create widgets
            createTransformWidgets(layout, nodes);
        }

        void Plugin::createTransformWidgets(QVBoxLayout *layout, QVector<SceneNode*> &nodes) {
            TransformWidget *w = new TransformWidget(nodes, tr("Transform"));
            layout->addWidget(w);

            w->toggleContentShown(true); //Default to visible
        }

    }
}

