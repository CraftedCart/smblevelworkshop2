/**
 * @file
 * @brief Header for the PropertiesWidget class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_PROPERTIESWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_PROPERTIESWIDGET_HPP

#include "ws2editor_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include <QVBoxLayout>
#include <QWidget>

namespace WS2Editor {
    namespace Widget {
        class WS2EDITOR_EXPORT PropertiesWidget : public QWidget {
            Q_OBJECT

            protected:
                QVBoxLayout *layout;

            public:
                explicit PropertiesWidget(QWidget *parent = nullptr);

            signals:
                void onUpdatePropertiesWidget(QVBoxLayout *layout, QVector<WS2Common::Scene::SceneNode*>& nodes);

            public slots:
                /**
                 * @brief Updates the properties widget to show details about given objects
                 *
                 * @param nodes A vector of objects to populate the properties widget with
                 */
                void updatePropertiesWidget(QVector<WS2Common::Scene::SceneNode*>& nodes);
        };
    }
}

#endif

