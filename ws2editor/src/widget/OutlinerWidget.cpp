#include "widget/OutlinerWidget.hpp"

namespace WS2Editor {
    namespace Widget {
        OutlinerWidget::OutlinerWidget(QWidget *parent) : QTreeView(parent) {}

        void OutlinerWidget::onSelectionChanged(QVector<QModelIndex> selectedObjects) {
            //Deselect all first
            clearSelection();

            QItemSelectionModel *selModel = selectionModel();

            for (int i = 0; i < selectedObjects.size(); i++) {
                selModel->select(QItemSelection(selectedObjects.at(i), selectedObjects.at(i)), QItemSelectionModel::Select);
            }

            setSelectionModel(selModel);
        }
    }
}

