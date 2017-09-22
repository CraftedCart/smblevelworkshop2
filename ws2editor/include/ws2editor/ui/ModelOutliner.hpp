/**
 * @file
 * @brief Header for the WS2Editor::UI::ModelOutliner clas
 */

#ifndef SMBLEVELWORKSHOP2_UI_MODELOUTLINER_HPP
#define SMBLEVELWORKSHOP2_UI_MODELOUTLINER_HPP

#include "ws2common/scene/SceneNode.hpp"
#include <QAbstractTableModel>

namespace WS2Editor {
    namespace UI {
        class ModelOutliner : public QAbstractTableModel {
            Q_OBJECT

            protected:
                /**
                 * @brief Getter for the root node of the active project
                 *
                 * @return A pointer to the root SceneNode of the active project
                 */
                WS2Common::Scene::SceneNode* getRootNode() const;

            public:
                ModelOutliner(QObject *parent);
                int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                int columnCount(const QModelIndex &parent = QModelIndex()) const override;
                QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
                QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
                QModelIndex parent(const QModelIndex &parent = QModelIndex()) const override;
                Qt::ItemFlags flags(const QModelIndex &index) const override;
                //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

                /**
                 * @brief Returns the model index for a given node
                 *
                 * @param node The node to find the model index for
                 *
                 * @return A QModelIndex corresponding to the node
                 */
                QModelIndex findIndexFromNode(WS2Common::Scene::SceneNode *node);

                /**
                 * @brief Adds an entry for a new node
                 *
                 * @param addedNode The newly added node
                 */
                void onNodeAdded(WS2Common::Scene::SceneNode *addedNode);

                /**
                 * @brief Call this when the selection changes
                 *
                 * @param selectedObjects A vector of selected objects
                 */
                void selectionChanged(QVector<WS2Common::Scene::SceneNode*>& selectedObjects);

            signals:
                /**
                 * @brief Emitted when the selection has been changed
                 *
                 * @param indices A vector of model indices
                 */
                void onSelectionChanged(QVector<QModelIndex> indices);
        };
    }
}

#endif

