/**
 * @file
 * @brief Header for the WS2Editor::Task::Task class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_TASK_TASK_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_TASK_TASK_HPP

#include "ws2editor/Progress.hpp"
#include <QString>

namespace WS2Editor {
    namespace Task {

        /**
         * @brief An executable task with progress reporting and a user friendly message
         */
        class Task {
            public:
                virtual ~Task() {}
                virtual void exec(Progress *prog) = 0;
                virtual QString getTranslatedMessage() = 0;
        };
    }
}

#endif

