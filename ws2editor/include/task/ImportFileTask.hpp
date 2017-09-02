/**
 * @file
 * @brief Header for the WS2::Task::ImportFileTask class
 */

#ifndef SMBLEVELWORKSHOP2_TASK_IMPORTFILETASK_HPP
#define SMBLEVELWORKSHOP2_TASK_IMPORTFILETASK_HPP

#include "task/Task.hpp"
#include <QFile>
#include <functional>

namespace WS2 {
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

