/**
 * @file
 * @brief Header for the OutlinerWidget class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_OUTLINERWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_OUTLINERWIDGET_HPP

#include "ws2common/scene/SceneNode.hpp"
#include <QTreeView>

namespace WS2Editor {
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
                void onSelectionChanged(QVector<QModelIndex> selectedObjects);
        };
    }
}

#endif

