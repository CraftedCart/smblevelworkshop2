#include "lzexportproviderplugin/SMB2RawLzExportProvider.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2lz/SMB2LzExporter.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include <QInputDialog>
#include <QBuffer>
#include <QDebug>

namespace WS2EditorPlugins {
    namespace LzExportProviderPlugin {
        using namespace WS2Editor::Project;
        using namespace WS2Editor::Resource;
        using namespace WS2Common::Resource;
        using namespace WS2Common;
        using namespace WS2Common::Scene;

        QString SMB2RawLzExportProvider::getTranslatedTypeName() {
            return tr("Super Monkey Ball 2 Raw LZ [*.lz.raw] (Needs to be compressed before usable in-game)");
        }

        QVector<QPair<QString, QString>> SMB2RawLzExportProvider::getNameFilters() {
            QPair<QString, QString> entry(tr("Uncompressed LZ (*.lz.raw)"), ".lz.raw");

            QVector<QPair<QString, QString>> vec = {entry};
            return vec;
        }

        Result<> SMB2RawLzExportProvider::checkProject(Project *project) {
            //Check if we have a start node
            bool hasStart = false;

            //Find the start
            for (SceneNode *node : project->getScene()->getRootNode()->getChildren()) {
                if (dynamic_cast<StartSceneNode*>(node)) {
                    hasStart = true;
                    break;
                }
            }

            if (hasStart) {
                //TODO: Super tempoary way of setting the fallout plane on export
                bool ok = false;
                float fallout = QInputDialog::getDouble(
                        nullptr,
                        tr("Set fallout Y"),
                        tr("Set fallout Y: (Also be sure to yell at Crafted for forgetting this initially)"),
                        project->getScene()->getStage()->getFalloutY(),
                        -FLT_MAX,
                        FLT_MAX,
                        2,
                        &ok
                        );

                if (ok) {
                    project->getScene()->getStage()->setFalloutY(fallout);
                    return Result<>(EnumStatus::SUCCESS);
                } else {
                    return Result<>(EnumStatus::FAILURE, tr("No fallout Y provided"));
                }
            } else {
                return Result<>(EnumStatus::FAILURE, tr("No player start found in the scene root"));
            }
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

