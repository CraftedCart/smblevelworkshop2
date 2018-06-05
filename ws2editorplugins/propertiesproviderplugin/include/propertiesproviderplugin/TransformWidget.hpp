/**
 * @file
 * @brief Transform widget for the properties panel
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_TRANSFORMWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_TRANSFORMWIDGET_HPP

#include "ws2editor/widget/CollapsableWidget.hpp"
#include "ws2common/scene/SceneNode.hpp"
#include <QVector>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        class TransformWidget : public WS2Editor::Widget::CollapsableWidget {
            Q_OBJECT

            public:
                TransformWidget(QVector<WS2Common::Scene::SceneNode*> &nodes, QString label = "", QWidget *parent = nullptr);
        };
    }
}

#endif

