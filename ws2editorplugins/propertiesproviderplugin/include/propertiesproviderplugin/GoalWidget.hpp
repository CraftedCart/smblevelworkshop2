/**
 * @file
 * @brief Goal widget for the properties panel
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_GOALWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_GOALWIDGET_HPP

#include "ws2editor/widget/CollapsableWidget.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include <QPushButton>
#include <QVector>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        class GoalWidget : public WS2Editor::Widget::CollapsableWidget {
            Q_OBJECT

            protected:
                QVector<WS2Common::Scene::GoalSceneNode*> selectedGoalNodes;

                QPushButton *blueTypeButton;
                QPushButton *greenTypeButton;
                QPushButton *redTypeButton;

            protected:
                void updateValues();

            public:
                GoalWidget(QVector<WS2Common::Scene::GoalSceneNode*> &nodes, QString label = "", QWidget *parent = nullptr);

            public slots:
                void onNodeModified(WS2Common::Scene::SceneNode *node);
                void onTypeModified(WS2Common::EnumGoalType newType);
        };
    }
}

#endif

