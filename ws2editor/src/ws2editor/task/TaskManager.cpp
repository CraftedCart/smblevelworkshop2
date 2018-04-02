#include "ws2editor/task/TaskManager.hpp"
#include "ws2editor/WS2Editor.hpp"
#include <QThreadPool>
#include <QDebug>

namespace WS2Editor {
    namespace Task {
        void TaskManager::enqueueTask(Task *task) {
            connect(task, &Task::onFinished, this, &TaskManager::onTaskFinished);
            task->setAutoDelete(false);
            task->setProgress(progress);
            QThreadPool::globalInstance()->start(task);

            runningTasks[task] = new Progress();
            emit messageChanged(getStatusString());
        }

        void TaskManager::enqueueTasks(QVector<Task*> &tasks) {
            for (Task *task : tasks) enqueueTask(task);
        }

        QString TaskManager::getStatusString() {
            if (runningTasks.size() > 1) {
                return tr("%n background task(s)", "", runningTasks.size());
            } else if (runningTasks.size() == 1) {
                return runningTasks.keys()[0]->getTranslatedMessage();
            } else {
                return tr("All tasks completed");
            }
        }

        void TaskManager::onTaskFinished(Task *obj) {
            delete runningTasks[obj]; //Delete the progress object associaed with the task
            runningTasks.remove(obj);
            emit messageChanged(getStatusString());

            delete obj;
        }
    }
}

