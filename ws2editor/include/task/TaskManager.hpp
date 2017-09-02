/**
 * @file
 * @brief Header for the WS2::Task::TaskManager class
 */

#ifndef SMBLEVELWORKSHOP2_TASK_TASKMANAGER_HPP
#define SMBLEVELWORKSHOP2_TASK_TASKMANAGER_HPP

#include "Task.hpp"
#include <QQueue>

namespace WS2 {
    namespace Task {
        class TaskManager : public QObject {
            Q_OBJECT

            protected:
                QQueue<Task*> taskQueue;
                Progress *progress = new Progress();

            public:
                TaskManager();

                /**
                 * @brief Queue a task to be executed
                 *
                 * @param task The task to queue up
                 */
                void enqueueTask(Task *task);

                /**
                 * @brief Queue multiple tasks to be executed
                 *
                 * @param tasks The tasks to queue up
                 */
                void enqueueTasks(QVector<Task*> &tasks);

                /**
                 * @brief Getter for progress
                 *
                 * @return A pointer to the progress object
                 */
                Progress* getProgress();

            signals:
                /**
                 * @brief Used to call execTasks() on the main thread
                 */
                void signalExecTasks();

                /**
                 * @brief Emitted when the task has changed and a user friendly message status needs to be updated
                 *
                 * @param msg The user friendly status message
                 */
                void messageChanged(QString msg);

            public slots:
                /**
                 * @brief Keeps executing tasks until there's no more to execute
                 */
                void execTasks();
        };
    }
}

#endif

