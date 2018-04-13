#include "ws2editor/ui/ModelOutliner.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/WS2Editor.hpp"
#include "ws2common/SerializeUtils.hpp"
#include <QDebug>

namespace WS2Editor {
    namespace UI {
        using namespace WS2Common::Scene;

        ModelOutliner::ModelOutliner(QObject *parent) : QAbstractTableModel(parent) {}

        SceneNode* ModelOutliner::getRootNode() const {
            return Project::ProjectManager::getActiveProject()->getScene()->getRootNode();
        }

        int ModelOutliner::rowCount(const QModelIndex &parent) const {
            SceneNode *parentNode;
            if (!parent.isValid()) {
                parentNode = getRootNode();
            } else {
                parentNode = static_cast<SceneNode*>(parent.internalPointer());
            }

            return parentNode->getChildCount();
        }

        int ModelOutliner::columnCount(const QModelIndex &parent) const {
            //Suppress compiler warnings
            Q_UNUSED(parent);

            return 1;
        }

        QVariant ModelOutliner::data(const QModelIndex &index, int role) const {
            if (!index.isValid() || role != Qt::DisplayRole) return QVariant();

            SceneNode *node = static_cast<SceneNode*>(index.internalPointer());
            return node->getName();
        }

        QModelIndex ModelOutliner::index(int row, int column, const QModelIndex &parent) const {
            if (!hasIndex(row, column, parent)) return QModelIndex();

            //Get the parent node
            SceneNode *parentNode;
            if (!parent.isValid()) {
                parentNode = getRootNode();
            } else {
                parentNode = static_cast<SceneNode*>(parent.internalPointer());
            }

            SceneNode *childNode = parentNode->getChildByIndex(row);
            if (childNode) {
                return createIndex(row, column, childNode);
            } else {
                return QModelIndex();
            }
        }

        QModelIndex ModelOutliner::parent(const QModelIndex &index) const {
            if (!index.isValid()) return QModelIndex();

            SceneNode *childNode = static_cast<SceneNode*>(index.internalPointer());
            SceneNode *parentNode = childNode->getParent();

            if (parentNode == getRootNode()) {
                return QModelIndex();
            } else {
                return createIndex(parentNode->getIndex(), 0, parentNode);
            }
        }

        Qt::ItemFlags ModelOutliner::flags(const QModelIndex &index) const {
            if (!index.isValid()) return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled;

            return QAbstractItemModel::flags(index) | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;
        }

        QStringList ModelOutliner::mimeTypes() const {
            QStringList types;
            types << "text/xml";
            return types;
        }

        QMimeData* ModelOutliner::mimeData(const QModelIndexList &indexes) const {
            QMimeData *mimeData = new QMimeData();
            QByteArray encodedData;
            QXmlStreamWriter stream(&encodedData);

            stream.setAutoFormatting(true);
            stream.writeStartDocument();

            stream.writeStartElement("root");
            for (const QModelIndex &index : indexes) {
                if (index.isValid()) {
                    static_cast<SceneNode*>(index.internalPointer())->serializeXml(stream);
                }
            }
            stream.writeEndElement();

            stream.writeEndDocument();

            mimeData->setData("text/xml", encodedData);
            return mimeData;
        }

        bool ModelOutliner::canDropMimeData(
                const QMimeData *data,
                Qt::DropAction action,
                int row,
                int column,
                const QModelIndex &parent
                ) const {
            Q_UNUSED(action);
            Q_UNUSED(row);
            Q_UNUSED(parent);

            if (!data->hasFormat("text/xml")) return false;
            if (column > 0) return false;

            return true;
        }

        bool ModelOutliner::dropMimeData(
                const QMimeData *data,
                Qt::DropAction action,
                int row,
                int column,
                const QModelIndex &parent
                ) {
            using namespace WS2Common;

            if (!canDropMimeData(data, action, row, column, parent))
                return false;

            if (action == Qt::IgnoreAction)
                return true;

            int beginRow;

            if (row != -1) {
                beginRow = row;
            } else if (parent.isValid()) {
                beginRow = parent.row();
            } else {
                beginRow = rowCount(QModelIndex());
            }

            QByteArray encodedData = data->data("text/xml");
            qDebug().noquote().nospace() << data->data("text/xml");
            QXmlStreamReader xml(encodedData);

            while (!(xml.isEndElement() && xml.name() == "root")) {
                //Keep reading until the </root> tag
                xml.readNext();
                if (!xml.isStartElement()) continue; //Ignore all end elements

                if (!xml.name().startsWith("node-")) {
                    if (xml.name() == "root") continue; //Don't yell at me in the logs if this is the root node
                    qCritical().noquote() << "Error when parsing node XML - Expected \"node-*\" but got \"" + xml.name() + "\"";
                    continue;
                }

                SceneNode *node = SerializeUtils::deserializeNodeFromXml(xml);

                if (node == nullptr) {
                    qCritical().noquote() << "Node deserialization returned nullptr when dropping";
                }

                if (parent.isValid()) {
                    addNode(node, static_cast<SceneNode*>(parent.internalPointer()));
                } else {
                    addNode(node, getRootNode());
                }
            }

            //insertRows(beginRow, rows, QModelIndex());

            //foreach (const QString &text, newItems) {
                //QModelIndex idx = index(beginRow, 0, QModelIndex());
                //setData(idx, text);
                //beginRow++;
            //}

            return true;
        }

        QModelIndex ModelOutliner::findIndexFromNode(SceneNode *node) {
            QVector<int> indexPath;
            SceneNode *pathNode = node;
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

        void ModelOutliner::addNode(SceneNode *node, SceneNode *parentNode) {
            //Check if the QApplication is running else we would crash
            if (WS2Editor::qAppRunning) {
                QModelIndex parentIndex = findIndexFromNode(parentNode);
                int index = parentNode->getChildCount();

                beginInsertRows(parentIndex, index, index);
            }

            parentNode->addChild(node);

            if (WS2Editor::qAppRunning) endInsertRows();
        }

        void ModelOutliner::addNodeWithMesh(SceneNode *node, SceneNode *parentNode, ResourceMesh *mesh) {
            addNode(node, parentNode);

            Project::ProjectManager::getActiveProject()->getScene()->addMeshNodeData(node, new MeshNodeData(node, mesh));
        }

        void ModelOutliner::removeNode(SceneNode *node) {
            //Remove nodes from the selection (If they are even selected)
            Project::ProjectManager::getActiveProject()->getScene()->getSelectionManager()->deselect(node);

            //Check if the QApplication is running else we would crash
            if (WS2Editor::qAppRunning) {
                QModelIndex index = findIndexFromNode(node);
                QModelIndex parentIndex = index.parent();
                int removedIndex = index.row();

                beginRemoveRows(parentIndex, removedIndex, removedIndex);
            }

            node->removeFromParent();

            //Also remove mesh data, if any exists
            Project::ProjectManager::getActiveProject()->getScene()->removeMeshNodeData(node);

            if (WS2Editor::qAppRunning) endRemoveRows();
        }

        void ModelOutliner::selectionChanged(QVector<SceneNode*>& selectedObjects, bool emitOnSelectionChanged) {
            QVector<QModelIndex> indices;

            for (int i = 0; i < selectedObjects.size(); i++) {
                QModelIndex modelIdx = findIndexFromNode(selectedObjects.at(i));
                indices.append(modelIdx);
            }

            if (emitOnSelectionChanged) emit onSelectionChanged(indices);
        }

    }
}

