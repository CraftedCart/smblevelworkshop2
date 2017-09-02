#include "task/TaskManager.hpp"
#include "WS2.hpp"

namespace WS2 {
    namespace Task {
        TaskManager::TaskManager() {
            //When signalExecTasks is emitted, run execTasks on the main thread
            connect(this, &TaskManager::signalExecTasks, this, &TaskManager::execTasks);
        }

        void TaskManager::enqueueTask(Task *task) {
            taskQueue.enqueue(task);

            QStack<Progress::ProgressOperation*>& opStack = progress->getOpStack();
            if (opStack.size() > 0) {
                opStack[0]->stepCount++;
                emit progress->maxChanged(progress->getMax());
            }

            //If this is the only task, start executing tasks
            if (taskQueue.size() == 1) emit signalExecTasks();
        }

        void TaskManager::enqueueTasks(QVector<Task*> &tasks) {
            for (int i = 0; i < tasks.size(); i++) taskQueue.enqueue(tasks[i]);

            QStack<Progress::ProgressOperation*>& opStack = progress->getOpStack();
            if (opStack.size() > 0) {
                opStack[0]->stepCount += tasks.size();
                emit progress->maxChanged(progress->getMax());
            }

            //If these are the only tasks, start executing tasks
            if (taskQueue.size() == tasks.size()) emit signalExecTasks();
        }

        void TaskManager::execTasks() {
            progress->begin(taskQueue.size());

            while (taskQueue.size() > 0) {
                Task *task = taskQueue.head();

                emit messageChanged(task->getTranslatedMessage());
                ws2App->processEvents();

                task->exec(progress);
                taskQueue.dequeue();
                delete task;

                progress->inc();
            }

            progress->end();

            emit messageChanged(tr("All tasks finished"));
        }

        Progress* TaskManager::getProgress() {
            return progress;
        }
    }
}

