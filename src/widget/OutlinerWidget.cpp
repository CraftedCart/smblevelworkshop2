#include "widget/OutlinerWidget.hpp"

namespace WS2 {
    OutlinerWidget::OutlinerWidget(QWidget *parent) : QTreeView(parent) {}

    void OutlinerWidget::selectionChanged(QVector<QModelIndex> selectedObjects) {
        //Deselect all first
        clearSelection();

        QItemSelectionModel *selModel = selectionModel();

        for (int i = 0; i < selectedObjects.size(); i++) {
            selModel->select(QItemSelection(selectedObjects.at(i), selectedObjects.at(i)), QItemSelectionModel::Select);
        }

        setSelectionModel(selModel);
    }
}

