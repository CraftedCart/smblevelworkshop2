/**
 * @file
 * @brief Header for the IEditorPlugin interface
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PLUGIN_IEDITORPLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PLUGIN_IEDITORPLUGIN_HPP

#include <QtPlugin>

namespace WS2Editor {
    namespace Plugin {
        class IEditorPlugin {
            public:
                virtual ~IEditorPlugin() = default;

                /**
                 * @brief Called when the plugin is first loaded
                 */
                virtual bool init() = 0;
        };
    }
}

#define WS2EDITOR_IEDITORPLUGIN_IID "craftedcart.smblevelworkshop2.ws2editor.plugin.IEditorPlugin"
Q_DECLARE_INTERFACE(WS2Editor::Plugin::IEditorPlugin, WS2EDITOR_IEDITORPLUGIN_IID)

#endif

