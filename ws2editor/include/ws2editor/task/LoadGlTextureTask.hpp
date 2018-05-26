/**
 * @file
 * @brief Header for the WS2Editor::Task::LoadGlTextureTask class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_TASK_LOADGLTEXTURETASK_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_TASK_LOADGLTEXTURETASK_HPP

#include "ws2editor_export.h"
#include "ws2editor/task/Task.hpp"
#include "ws2common/resource/ResourceTexture.hpp"
#include <QImage>

namespace WS2Editor {
    namespace Task {
        using namespace WS2Common::Resource;

        class WS2EDITOR_EXPORT LoadGlTextureTask : public Task {
            Q_OBJECT

            protected:
                const ResourceTexture &tex;

            public:
                LoadGlTextureTask(const ResourceTexture &tex);

                void runTask(Progress *prog) override;
                QString getTranslatedMessage() override;

            signals:
                void addTexture(const QImage loadedImage, const ResourceTexture *tex);
        };
    }
}

#endif

