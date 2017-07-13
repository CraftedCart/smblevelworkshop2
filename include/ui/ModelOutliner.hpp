/**
 * @file
 * @brief Header for the WS2::UI::ModelOutliner clas
 */

#ifndef SMBLEVELWORKSHOP2_UI_MODELOUTLINER_HPP
#define SMBLEVELWORKSHOP2_UI_MODELOUTLINER_HPP

#include "scene/SceneNode.hpp"
#include <QAbstractTableModel>

namespace WS2 {
    namespace UI {
        class ModelOutliner : public QAbstractTableModel {
            Q_OBJECT

            protected:
                /**
                 * @brief Getter for the root node of the active project
                 *
                 * @return A pointer to the root SceneNode of the active project
                 */
                Scene::SceneNode* getRootNode() const;

            public:
                ModelOutliner(QObject *parent);
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                int columnCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
                QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
                QModelIndex parent(const QModelIndex &parent = QModelIndex()) const override;
                //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

                void onNodeAdded(Scene::SceneNode *addedNode);
        };
    }
}

#endif

