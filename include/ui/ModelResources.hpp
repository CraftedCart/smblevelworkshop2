/**
 * @file
 * @brief Header for the WS2::UI::ResourceModel clas
 */

#ifndef SMBLEVELWORKSHOP2_UI_MODELRESOURCES_HPP
#define SMBLEVELWORKSHOP2_UI_MODELRESOURCES_HPP

#include "resource/AbstractResource.hpp"
#include <QAbstractTableModel>

namespace WS2 {
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

