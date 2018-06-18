/**
 * @file
 * @brief ws2editor raw LZ export provider for Super Monkey Ball 2
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_LZEXPORTPROVIDERPLUGIN_SMB2RAWLZEXPORTPROVIDER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_LZEXPORTPROVIDERPLUGIN_SMB2RAWLZEXPORTPROVIDER_HPP

#include "ws2editor/plugin/IEditorPlugin.hpp"
#include "ws2editor/IExportProvider.hpp"

namespace WS2EditorPlugins {
    namespace LzExportProviderPlugin {
        class SMB2RawLzExportProvider : public WS2Editor::IExportProvider {
            Q_OBJECT

            public:
                virtual QString getTranslatedTypeName() override;
                virtual QVector<QPair<QString, QString>> getNameFilters() override;
        };
    }
}

#endif

