#include "ui/ModelResources.hpp"
#include "resource/ResourceManager.hpp"
#include <QStringList>

namespace WS2 {
    namespace UI {
        ModelResources::ModelResources(QObject *parent) : QAbstractTableModel(parent) {}

        int ModelResources::rowCount(const QModelIndex &parent) const {
            return Resource::ResourceManager::getResources().size();
        }

        int ModelResources::columnCount(const QModelIndex &parent) const {
            return 2;
        }

        QVariant ModelResources::data(const QModelIndex &index, int role) const {
            if (role == Qt::DisplayRole) {
                Resource::AbstractResource* res = Resource::ResourceManager::getResources().at(index.row());

                switch (index.column()) {
                    case 0:
                        //Resource ID
                        return res->getId();
                    case 1:
                        //Resource origin(s)
                        QStringList list;
                        for (int i = 0; i < res->getFilePaths().size(); i++) {
                            list << res->getFilePaths().at(i);
                        }

                        return list.join(", ");
                }

                //This shouldn't happen
                return QVariant();
            } else {
                return QVariant();
            }
        }

        QVariant ModelResources::headerData(int section, Qt::Orientation orientation, int role) const {
            if (role == Qt::DisplayRole) {
                if (orientation == Qt::Horizontal) {
                    switch (section) {
                        case 0:
                            return tr("Resource ID");
                            break;
                        case 1:
                            return tr("Resource origin(s)");
                            break;
                    }
                }
            }

            //Nothing above matched - Return a QVariant
            return QVariant();
        }

        void ModelResources::onResourceAdded() {
            int rows = Resource::ResourceManager::getResources().size();
            beginInsertRows(QModelIndex(), rows - 1, rows - 1);
            endInsertRows();
        }
    }
}

