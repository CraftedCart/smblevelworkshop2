/**
 * @file
 * @brief Header for the OutlinerWidget class
 */

#ifndef SMBLEVELWORKSHOP2_WIDGET_OUTLINERWIDGET_HPP
#define SMBLEVELWORKSHOP2_WIDGET_OUTLINERWIDGET_HPP

#include "scene/SceneNode.hpp"
#include <QTreeView>

namespace WS2 {
    namespace Widget {
        class OutlinerWidget : public QTreeView {
            Q_OBJECT

            public:
                explicit OutlinerWidget(QWidget *parent = nullptr);

            public slots:
                /**
                 * @brief Updates the outliner selection
                 *
                 * @param selectedObjects A vector of the model indices for the selected objects
                 */
                void selectionChanged(QVector<QModelIndex> selectedObjects);
        };
    }
}

#endif

