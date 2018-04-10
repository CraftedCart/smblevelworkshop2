/**
 * @file
 * @brief Header for the WS2Editor::UI::ModelOutliner clas
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELOUTLINER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELOUTLINER_HPP

#include "ws2common/scene/SceneNode.hpp"
#include <QAbstractTableModel>

namespace WS2Editor {
    namespace UI {
        using namespace WS2Common::Scene;

        class ModelOutliner : public QAbstractTableModel {
            Q_OBJECT

            protected:
                /**
                 * @brief Getter for the root node of the active project
                 *
                 * @return A pointer to the root SceneNode of the active project
                 */
                SceneNode* getRootNode() const;

            public:
                ModelOutliner(QObject *parent);
                virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
                virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
                virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
                virtual QModelIndex parent(const QModelIndex &parent = QModelIndex()) const override;
                virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
                //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

                /**
                 * @brief Returns the model index for a given node
                 *
                 * @param node The node to find the model index for
                 *
                 * @return A QModelIndex corresponding to the node
                 */
                QModelIndex findIndexFromNode(SceneNode *node);

                /**
                 * @brief Adds a node to the parent and updates the model
                 *
                 * @param node The new node to add
                 * @param parentNode The node to parent the new node to
                 */
                void addNode(SceneNode *node, SceneNode *parentNode);

                /**
                 * @brief Removes a node from its parent and updates the model
                 *
                 * @param node The new node to remove
                 */
                void removeNode(SceneNode *node);

                /**
                 * @brief Call this when the selection changes
                 *
                 * @param selectedObjects A vector of selected objects
                 * @param emitOnSelectionChanged Should onSelectionChanged be emitted? (Used to prevent inf. loops)
                 */
                void selectionChanged(QVector<SceneNode*>& selectedObjects, bool emitOnSelectionChanged = true);

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

