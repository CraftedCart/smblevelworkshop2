#include "ws2editor/IExportProvider.hpp"

namespace WS2Editor {
    using namespace WS2Common;

    Result<void, QString> IExportProvider::checkProject(Project::Project *project) {
        Q_UNUSED(project);

        return Ok();
    }
}

