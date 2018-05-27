/**
 * @file
 * @brief Header for the WS2Editor::Task::ImportFileTask class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_TASK_IMPORTFILETASK_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_TASK_IMPORTFILETASK_HPP

#include "ws2editor_export.h"
#include "ws2editor/task/Task.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QFile>

namespace WS2Editor {
    namespace Task {
        using namespace WS2Common::Resource;

        class WS2EDITOR_EXPORT ImportFileTask : public Task {
            Q_OBJECT

            protected:
                QFile *f;

            public:
                /**
                 * @brief Constructor for ImportFileTask
                 *
                 * @param f The file to import
                 * @param contextFunc A function called before importing - use it to make the rendering context current
                 */
                ImportFileTask(QFile *f);

                /**
                 * @brief Deletes the file passed into the function
                 */
                ~ImportFileTask();

                void runTask(Progress *prog) override;
                QString getTranslatedMessage() override;

            signals:
                void addModel(const QVector<ResourceMesh*> newMeshes);
        };
    }
}

#endif

