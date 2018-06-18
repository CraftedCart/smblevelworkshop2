#include "lzexportproviderplugin/SMB2RawLzExportProvider.hpp"

namespace WS2EditorPlugins {
    namespace LzExportProviderPlugin {
        QString SMB2RawLzExportProvider::getTranslatedTypeName() {
            return tr("Super Monkey Ball 2 Raw LZ [*.lz.raw] (Needs to be compressed before usable in-game)");
        }

        QVector<QPair<QString, QString>> SMB2RawLzExportProvider::getNameFilters() {
            QPair<QString, QString> entry(tr("Uncompressed LZ (*.lz.raw)"), ".lz.raw");

            QVector<QPair<QString, QString>> vec = {entry};
            return vec;
        }
    }
}

