#include "ws2editor/task/LoadGlTextureTask.hpp"
#include "ws2editor/RenderManager.hpp"
#include "ws2editor/task/TaskManager.hpp"

namespace WS2Editor {
    namespace Task {
        using namespace WS2Common::Resource;

        LoadGlTextureTask::LoadGlTextureTask(const ResourceTexture &tex) :
            Task(),
            tex(tex) {}

        void LoadGlTextureTask::runTask(Progress *prog) {
            QImage img(*tex.getFirstFilePath());
            emit addTexture(RenderManager::convertToGLFormat(img), &tex);
        }

        QString LoadGlTextureTask::getTranslatedMessage() {
            return TaskManager::tr("Loading GL texture: %1").arg(*tex.getFirstFilePath());
        }
    }
}

