/**
 * @file
 * @brief Header for the Task class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_TASK_TASK_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_TASK_TASK_HPP

#include "ws2editor_export.h"
#include "ws2editor/Progress.hpp"
#include <QRunnable>

namespace WS2Editor {
    namespace Task {

        /**
         * @brief An executable task with progress reporting and a user friendly message
         */
        class WS2EDITOR_EXPORT Task : public QObject, public QRunnable {
            Q_OBJECT

            protected:
                /**
                 * @brief Used for progress tracking (Optional - may be nullptr)
                 */
                Progress *prog;

            public:
                virtual ~Task() {}

                /**
                 * @brief Override this and place your task execution code in here
                 *
                 * @param prog Used for progress tracking - may be nullptr
                 */
                virtual void runTask(Progress *prog) = 0;

                virtual QString getTranslatedMessage() = 0;

                virtual void run() override;

                /**
                 * @brief Used for progress tracking
                 *
                 * @param prog The progress object to write to while the task runs (optional)
                 */
                void setProgress(Progress *prog);

            signals:
                void onFinished(Task *obj);
        };
    }
}

#endif

