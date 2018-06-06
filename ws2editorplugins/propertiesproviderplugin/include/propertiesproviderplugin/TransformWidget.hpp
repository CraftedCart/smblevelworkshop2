/**
 * @file
 * @brief Transform widget for the properties panel
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_TRANSFORMWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_PROPERTIESPROVIDERPLUGIN_TRANSFORMWIDGET_HPP

#include "ws2editor/widget/CollapsableWidget.hpp"
#include "ws2editor/widget/Vec3DraggableSpinBoxes.hpp"
#include "ws2common/scene/SceneNode.hpp"
#include <QVector>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        class TransformWidget : public WS2Editor::Widget::CollapsableWidget {
            Q_OBJECT

            protected:
                QVector<WS2Common::Scene::SceneNode*> selectedNodes;

                glm::vec3 prevPos;
                WS2Editor::Widget::Vec3DraggableSpinBoxes *posSpinBoxes;
                glm::vec3 prevRot;
                WS2Editor::Widget::Vec3DraggableSpinBoxes *rotSpinBoxes;
                glm::vec3 prevScl;
                WS2Editor::Widget::Vec3DraggableSpinBoxes *sclSpinBoxes;

            protected:
                void updateValues();

            public:
                TransformWidget(QVector<WS2Common::Scene::SceneNode*> &nodes, QString label = "", QWidget *parent = nullptr);

            public slots:
                void onNodeModified(WS2Common::Scene::SceneNode *node);
                void onPosModified(glm::vec3 newValue);
                void onRotModified(glm::vec3 newValue);
                void onSclModified(glm::vec3 newValue);
        };
    }
}

#endif

