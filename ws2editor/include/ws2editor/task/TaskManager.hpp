/**
 * @file
 * @brief Header for the WS2Editor::Task::TaskManager class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_TASK_TASKMANAGER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_TASK_TASKMANAGER_HPP

#include "ws2editor_export.h"
#include "Task.hpp"
#include <QQueue>
#include <QHash>

namespace WS2Editor {
    namespace Task {
        class WS2EDITOR_EXPORT TaskManager : public QObject {
            Q_OBJECT

            protected:
                Progress *progress = new Progress();

                /**
                 * @brief A vector of tasks that are currently running async/queued up to run soon
                 *
                 * @tparam Task* The task to perform
                 * @tparam Progress* A progress tracker for the task
                 */
                QHash<Task*, Progress*> runningTasks;

            public:
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

                QString getStatusString();

            signals:
                /**
                 * @brief Emitted when the task has changed and a user friendly message status needs to be updated
                 *
                 * @param msg The user friendly status message
                 */
                void messageChanged(QString msg);

            public slots:
                /**
                 * @brief Removes the task from the runningTasks vector and emits messageChanged
                 *
                 * @param obj The task that just finished
                 */
                void onTaskFinished(Task *obj);
        };
    }
}

#endif

