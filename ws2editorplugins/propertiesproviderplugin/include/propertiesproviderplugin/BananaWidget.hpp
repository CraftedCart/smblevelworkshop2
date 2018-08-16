/**
 * @file
 * @brief Banana widget for the properties panel
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_BANANAWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_BANANAWIDGET_HPP

#include "ws2editor/widget/CollapsableWidget.hpp"
#include "ws2editor/RenderManager.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include <QPushButton>
#include <QVector>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        class BananaWidget : public WS2Editor::Widget::CollapsableWidget {
            Q_OBJECT

            protected:
                //Used to fetch banana models when reconstructing the mesh data
                WS2Editor::RenderManager *renderManager;

                QVector<WS2Common::Scene::BananaSceneNode*> selectedBananaNodes;

                QPushButton *singleTypeButton;
                QPushButton *bunchTypeButton;

            protected:
                void updateValues();

            public:
                BananaWidget(
                        QVector<WS2Common::Scene::BananaSceneNode*> &nodes,
                        WS2Editor::RenderManager *renderManager,
                        QString label = "",
                        QWidget *parent = nullptr
                        );

            public slots:
                void onNodeModified(WS2Common::Scene::SceneNode *node);
                void onTypeModified(WS2Common::EnumBananaType newType);
        };
    }
}

#endif

