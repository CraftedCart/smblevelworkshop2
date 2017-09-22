/**
 * @file
 * @brief Header for the WS2Editor::Task::ImportFileTask class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_TASK_IMPORTFILETASK_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_TASK_IMPORTFILETASK_HPP

#include "ws2editor/task/Task.hpp"
#include <QFile>
#include <functional>

namespace WS2Editor {
    namespace Task {
        class ImportFileTask : public Task {
            protected:
                QFile *f;
                std::function<void()>* contextFunc;

            public:
                ImportFileTask(QFile *f, std::function<void()>* contextFunc);
                ~ImportFileTask();
                void exec(Progress *prog) override;
                QString getTranslatedMessage() override;
        };
    }
}

#endif

