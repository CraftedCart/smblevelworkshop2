/**
 * @file
 * @brief Header for the WS2Editor::UI::ModelResources clas
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELRESOURCES_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELRESOURCES_HPP

#include "ws2common/resource/AbstractResource.hpp"
#include <QAbstractTableModel>

namespace WS2Editor {
    namespace UI {
        class ModelResources : public QAbstractTableModel {
            Q_OBJECT

            public:
                ModelResources(QObject *parent);
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                int columnCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
                QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

                void onResourceAdded();
        };
    }
}

#endif

