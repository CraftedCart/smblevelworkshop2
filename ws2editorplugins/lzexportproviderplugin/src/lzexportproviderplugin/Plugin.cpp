#include "lzexportproviderplugin/Plugin.hpp"
#include "lzexportproviderplugin/SMB2RawLzExportProvider.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include <QDebug>

namespace WS2EditorPlugins {
    namespace LzExportProviderPlugin {
        using namespace WS2Editor;

        bool Plugin::init() {
            //Create and register our export provider
            WS2EditorInstance::getInstance()->registerExportProvider(new SMB2RawLzExportProvider());

            //The plugin initialized successfully, return true
            qInfo() << "WS2Editor LZExportProviderPlugin successfully initialized";
            return true;
        }
    }
}

