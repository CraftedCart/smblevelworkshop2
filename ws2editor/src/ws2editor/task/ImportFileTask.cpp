#include "ws2editor/task/ImportFileTask.hpp"
#include "ws2editor/task/TaskManager.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2common/exception/ModelLoadingException.hpp"
#include <QDebug>

namespace WS2Editor {
    namespace Task {
        ImportFileTask::ImportFileTask(QFile *f) :
            Task(),
            f(f) {
            using namespace WS2Common::Resource;
            using namespace WS2Editor::Resource;

            ResourceScene *scene = Project::ProjectManager::getActiveProject()->getScene();
            connect(this, &ImportFileTask::addModel, scene, &ResourceScene::addModel);
        }

        ImportFileTask::~ImportFileTask() {
            delete f;
        }

        void ImportFileTask::runTask(Progress *prog) {
            using namespace WS2Common::Resource;
            using namespace WS2Common::Exception;
            using namespace WS2Editor::Resource;

            ResourceScene *scene = Project::ProjectManager::getActiveProject()->getScene();

            try {
                const QVector<ResourceMesh*> newMeshes = ResourceManager::addModel(*f, scene->isLoaded());

                emit addModel(newMeshes);
            } catch (const ModelLoadingException &e) {
                qCritical().noquote() << "Error while loading model:" << e.getMessage();

                //TODO: Show a message dialog or something
            }
        }

        QString ImportFileTask::getTranslatedMessage() {
            return TaskManager::tr("Importing file: %1").arg(f->fileName());
        }
    }
}

