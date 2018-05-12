/**
 * @file
 * @brief Example plugin header
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_WS2EDITOREXAMPLEPLUGIN_PLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_WS2EDITOREXAMPLEPLUGIN_PLUGIN_HPP

#include "ws2editor/plugin/IEditorPlugin.hpp"

namespace WS2EditorExamplePlugin {

    /**
     * @note Note that QObject is listed before IEditorPlugin!
     */
    class Plugin : public QObject, public WS2Editor::Plugin::IEditorPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID WS2EDITOR_IEDITORPLUGIN_IID)
        Q_INTERFACES(WS2Editor::Plugin::IEditorPlugin)

        public:
            virtual bool init() override;
    };
}

#endif

