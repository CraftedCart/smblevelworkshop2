#include "ui/ModelOutliner.hpp"
#include "project/ProjectManager.hpp"
#include "scene/MeshSceneNode.hpp"
#include "WS2.hpp"

namespace WS2 {
    namespace UI {
        ModelOutliner::ModelOutliner(QObject *parent) : QAbstractTableModel(parent) {}

        Scene::SceneNode* ModelOutliner::getRootNode() const {
            return Project::ProjectManager::getActiveProject()->getScene()->getRootNode();
        }

        int ModelOutliner::rowCount(const QModelIndex &parent) const {
            Scene::SceneNode *parentNode;
            if (!parent.isValid()) {
                parentNode = getRootNode();
            } else {
                parentNode = static_cast<Scene::SceneNode*>(parent.internalPointer());
            }

            return parentNode->getChildCount();
        }

        int ModelOutliner::columnCount(const QModelIndex &parent) const {
            return 1;
        }

        QVariant ModelOutliner::data(const QModelIndex &index, int role) const {
            if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

            Scene::SceneNode *node = static_cast<Scene::SceneNode*>(index.internalPointer());
            return node->getName();
        }

        QModelIndex ModelOutliner::index(int row, int column, const QModelIndex &parent) const {
            if (!hasIndex(row, column, parent)) return QModelIndex();

            //Get the parent node
            Scene::SceneNode *parentNode;
            if (!parent.isValid()) {
                parentNode = getRootNode();
            } else {
                parentNode = static_cast<Scene::SceneNode*>(parent.internalPointer());
            }

            Scene::SceneNode *childNode = parentNode->getChildByIndex(row);
            if (childNode) {
                return createIndex(row, column, childNode);
            } else {
                return QModelIndex();
            }
        }

        QModelIndex ModelOutliner::parent(const QModelIndex &index) const {
            if (!index.isValid()) return QModelIndex();

            Scene::SceneNode *childNode = static_cast<Scene::SceneNode*>(index.internalPointer());
            Scene::SceneNode *parentNode = childNode->getParent();

            if (parentNode == getRootNode()) {
                return QModelIndex();
            } else {
                return createIndex(parentNode->getIndex(), 0, parentNode);
            }
        }

        Qt::ItemFlags ModelOutliner::flags(const QModelIndex &index) const {
            if (!index.isValid()) return 0;
            return QAbstractItemModel::flags(index);
        }

        QModelIndex ModelOutliner::findIndexFromNode(Scene::SceneNode *node) {
            QVector<int> indexPath;
            Scene::SceneNode *pathNode = node;
            while (pathNode->getParent() != nullptr) {
                indexPath.append(pathNode->getIndex());
                pathNode = pathNode->getParent();
            }

            QModelIndex modelIdx;
            for (int i = indexPath.size() - 1; i >= 0; i--) {
                modelIdx = index(indexPath.at(i), 0, modelIdx);
            }

            return modelIdx;
        }

        void ModelOutliner::onNodeAdded(Scene::SceneNode *addedNode) {
            if (!WS2::qAppRunning) return; //Get outta here if the QApplication isn't running - This would crash otherwise

            QVector<int> indexPath;
            Scene::SceneNode *pathNode = addedNode;
            while (pathNode->getParent() != nullptr) {
                indexPath.append(pathNode->getIndex());
                pathNode = pathNode->getParent();
            }

            QModelIndex modelIdx;
            for (int i = 1; i < indexPath.size(); i++) {
                modelIdx = index(indexPath.at(i), 0, modelIdx);
            }

            int idx = addedNode->getIndex();
            beginInsertRows(modelIdx, idx, idx);
            endInsertRows();
        }

        void ModelOutliner::selectionChanged(QVector<Scene::SceneNode*>& selectedObjects) {
            QVector<QModelIndex> indices;

            for (int i = 0; i < selectedObjects.size(); i++) {
                QModelIndex modelIdx = findIndexFromNode(selectedObjects.at(i));
                indices.append(modelIdx);
            }

            emit onSelectionChanged(indices);
        }

    }
}

