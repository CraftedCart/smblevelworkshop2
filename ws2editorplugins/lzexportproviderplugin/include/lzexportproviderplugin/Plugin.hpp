/**
 * @file
 * @brief LZ export provider plugin header
 *
 * Provides the LZ exporter for ws2editor
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_LZEXPORTPROVIDERPLUGIN_PLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_LZEXPORTPROVIDERPLUGIN_PLUGIN_HPP

#include "ws2editor/plugin/IEditorPlugin.hpp"

namespace WS2EditorPlugins {
    namespace LzExportProviderPlugin {
        class Plugin : public QObject, public WS2Editor::Plugin::IEditorPlugin {
            Q_OBJECT

            //Required macros to tell Qt about our plugin
            Q_PLUGIN_METADATA(IID WS2EDITOR_IEDITORPLUGIN_IID)
            Q_INTERFACES(WS2Editor::Plugin::IEditorPlugin)

            public:
                /**
                 * @brief The init function is called for each plugin while ws2editor is loading
                 *
                 * @return Whether this plugin initialized successfully or not
                 */
                virtual bool init() override;
        };
    }
}

#endif

