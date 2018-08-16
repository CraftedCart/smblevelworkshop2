/**
 * @file
 * @brief Example plugin header
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_WS2EDITOREXAMPLEPLUGIN_PLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_WS2EDITOREXAMPLEPLUGIN_PLUGIN_HPP

#include "ws2editor/plugin/IEditorPlugin.hpp"
#include "ws2editor/WS2EditorInstance.hpp"

namespace WS2EditorExamplePlugin {

    /**
     * @brief The example plugin class. Every ws2editor plugin must have a class that extends QObject and IEditorPlugin.
     *
     * @note Note that QObject is listed before IEditorPlugin in the inheritence order!
     */
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

        public slots:
            /**
             * @brief Called after the StageEditorWindow has been constructed, so that the plugin can get a handle
             *        on the window and hook into its functions/events there
             *
             * @param w The StageEditorWindow
             */
            void onStageEditorWindowConstructed(WS2Editor::UI::StageEditorWindow &w);

            /**
             * @brief Shows a message box greeting the user
             */
            void showMessage();
    };
}

#endif

