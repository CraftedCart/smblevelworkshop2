/**
 * @file
 * @brief Header for the ModelOutliner class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELOUTLINER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_MODELOUTLINER_HPP

#include "ws2editor_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QAbstractTableModel>
#include <QMimeData>

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT ModelOutliner : public QAbstractTableModel {
            Q_OBJECT

            protected:
                /**
                 * @brief Getter for the root node of the active project
                 *
                 * @return A pointer to the root WS2Common::Scene::SceneNode of the active project
                 */
                WS2Common::Scene::SceneNode* getRootNode() const;

                /**
                 * @brief Finds mesh node data entries with matching UUIDs recursively and sets the node to the one given
                 *
                 * @param node The new node to make the owner of
                 */
                void recursiveTransferMeshNodeDataOwner(WS2Common::Scene::SceneNode *node);

                /**
                 * @brief Finds mesh node data matching the node's UUID and removes it if the node owner matched the one given
                 *
                 * @param node The node to check against
                 */
                void recursiveConditionalDestroyMeshNodeData(WS2Common::Scene::SceneNode *node);

            public:
                ModelOutliner(QObject *parent);
                virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
                virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
                virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
                virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
                virtual QModelIndex parent(const QModelIndex &parent = QModelIndex()) const override;
                virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
                //QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
                virtual QStringList mimeTypes() const override;
                virtual QMimeData* mimeData(const QModelIndexList &indexes) const override;
                virtual bool canDropMimeData(
                        const QMimeData *data,
                        Qt::DropAction action,
                        int row,
                        int column,
                        const QModelIndex &parent
                        ) const override;
                virtual bool dropMimeData(
                        const QMimeData *data,
                        Qt::DropAction action,
                        int row,
                        int column,
                        const QModelIndex &parent
                        ) override;
                virtual Qt::DropActions supportedDropActions() const override;
                virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

                /**
                 * @brief Returns the model index for a given node
                 *
                 * @param node The node to find the model index for
                 *
                 * @return A QModelIndex corresponding to the node
                 */
                QModelIndex findIndexFromNode(WS2Common::Scene::SceneNode *node);

                /**
                 * @brief Adds a node to the parent and updates the model
                 *
                 * @param node The new node to add
                 * @param parentNode The node to parent the new node to
                 */
                void addNode(WS2Common::Scene::SceneNode *node, WS2Common::Scene::SceneNode *parentNode);

                /**
                 * @brief Adds a node to the parent, registers the mesh with the scene, and updates the model
                 *
                 * @param node The new node to add
                 * @param parentNode The node to parent the new node to
                 * @param mesh The mesh to register with the scene
                 */
                void addNodeWithMesh(
                        WS2Common::Scene::SceneNode *node,
                        WS2Common::Scene::SceneNode *parentNode,
                        WS2Common::Resource::ResourceMesh *mesh
                        );

                /**
                 * @brief Adds a node to the parent, registers the meshes with the scene, and updates the model
                 *
                 * @param node The new node to add
                 * @param parentNode The node to parent the new node to
                 * @param meshes The meshes to register with the scene
                 */
                void addNodeWithMeshes(
                        WS2Common::Scene::SceneNode *node,
                        WS2Common::Scene::SceneNode *parentNode,
                        QVector<WS2Common::Resource::ResourceMesh*> meshes
                        );

                /**
                 * @brief Removes a node from its parent and updates the model
                 *
                 * @param node The new node to remove
                 */
                void removeNode(WS2Common::Scene::SceneNode *node);

                /**
                 * @brief Informs that model that a node has been changed, and updates the node + children recursively
                 *
                 * @param node The modified node
                 */
                void nodeModified(WS2Common::Scene::SceneNode *node);

                /**
                 * @brief Call this when the selection changes
                 *
                 * @param selectedObjects A vector of selected objects
                 * @param emitOnSelectionChanged Should onSelectionChanged be emitted? (Used to prevent inf. loops)
                 */
                void selectionChanged(
                        QVector<WS2Common::Scene::SceneNode*>& selectedObjects,
                        bool emitOnSelectionChanged = true
                        );

            signals:
                /**
                 * @brief Emitted when the selection has been changed
                 *
                 * @param indices A vector of model indices
                 */
                void onSelectionChanged(QVector<QModelIndex> indices);

                /**
                 * @brief Emitted when a nodeModified call occurs
                 *
                 * If a nodeModified request has been recieved, this signal will be emitted for the node and all its
                 * children recursively, in reverse order (Children first, before parents)
                 *
                 * @param node The modified node
                 */
                void onNodeModified(WS2Common::Scene::SceneNode *node);
        };
    }
}

#endif

