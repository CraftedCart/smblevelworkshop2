#include "lzexportproviderplugin/SMB2RawLzExportProvider.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2lz/SMB2LzExporter.hpp"
#include <QBuffer>
#include <QDebug>

namespace WS2EditorPlugins {
    namespace LzExportProviderPlugin {
        using namespace WS2Editor::Project;
        using namespace WS2Editor::Resource;
        using namespace WS2Common::Resource;

        QString SMB2RawLzExportProvider::getTranslatedTypeName() {
            return tr("Super Monkey Ball 2 Raw LZ [*.lz.raw] (Needs to be compressed before usable in-game)");
        }

        QVector<QPair<QString, QString>> SMB2RawLzExportProvider::getNameFilters() {
            QPair<QString, QString> entry(tr("Uncompressed LZ (*.lz.raw)"), ".lz.raw");

            QVector<QPair<QString, QString>> vec = {entry};
            return vec;
        }

        void SMB2RawLzExportProvider::exportFiles(QStringList targetFiles, Project *project) {
            //Fetch models from the resource manager
            QHash<QString, ResourceMesh*> models; //name, mesh

            for (AbstractResource *res : ResourceManager::getResources()) {
                //Check if resources are meshes
                if (ResourceMesh *mesh = dynamic_cast<ResourceMesh*>(res)) {
                    models[mesh->getId()] = mesh; //TODO: I should probably store mesh names in ResourceMesh
                }
            }

            //Generate an LZ
            QBuffer buf;
            buf.open(QIODevice::ReadWrite);
            QDataStream dStream(&buf);

            WS2Lz::SMB2LzExporter exporter;

            exporter.setModels(models);
            exporter.generate(dStream, *project->getScene()->getStage());

            //Write it to each target file
            for (QString file : targetFiles) {
                QFile o(file);
                o.open(QIODevice::WriteOnly);
                o.write(buf.data());
                o.close();
            }
        }
    }
}

