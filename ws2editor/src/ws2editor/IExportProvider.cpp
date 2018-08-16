#include "ws2editor/IExportProvider.hpp"

namespace WS2Editor {
    using namespace WS2Common;

    Result<> IExportProvider::checkProject(Project::Project *project) {
        Q_UNUSED(project);

        return Result<>(EnumStatus::SUCCESS);
    }
}

