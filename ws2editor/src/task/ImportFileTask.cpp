#include "task/ImportFileTask.hpp"
#include "task/TaskManager.hpp"
#include "project/ProjectManager.hpp"

namespace WS2Editor {
    namespace Task {
        /**
         * @brief Constructor for ImportFileTask
         *
         * @param f The file to import
         * @param contextFunc A function called before importing - use it to make the rendering context current
         */
        ImportFileTask::ImportFileTask(QFile *f, std::function<void()>* contextFunc) :
            f(f), contextFunc(contextFunc) {}

        ImportFileTask::~ImportFileTask() {
            delete f;
            delete contextFunc;
        }

        void ImportFileTask::exec(Progress *prog) {
            (*contextFunc)();
            Project::ProjectManager::getActiveProject()->importFile(*f);
        }

        QString ImportFileTask::getTranslatedMessage() {
            return TaskManager::tr("Importing file: %1").arg(f->fileName());
        }
    }
}

