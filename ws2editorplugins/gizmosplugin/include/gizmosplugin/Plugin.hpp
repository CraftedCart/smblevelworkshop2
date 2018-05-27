/**
 * @file
 * @brief Example plugin header
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_PLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_PLUGIN_HPP

#include "ws2editor/plugin/IEditorPlugin.hpp"
#include "ws2editor/WS2EditorInstance.hpp"

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        class Plugin : public QObject, public WS2Editor::Plugin::IEditorPlugin {
            Q_OBJECT

            //Required macros to tell Qt about our plugin
            Q_PLUGIN_METADATA(IID WS2EDITOR_IEDITORPLUGIN_IID)
            Q_INTERFACES(WS2Editor::Plugin::IEditorPlugin)

            private:
                WS2Editor::Widget::ViewportWidget *viewportWidget;
                WS2Editor::RenderManager *renderManager;

                //Gizmo collision bounds & highlight
                bool isGizmoPhysicsInWorld = false;
                WS2Editor::Physics::PhysicsContainer *gizmoYPhysics;
                bool highlightGizmoY;
                WS2Editor::Physics::PhysicsContainer *gizmoXPhysics;
                bool highlightGizmoX;
                WS2Editor::Physics::PhysicsContainer *gizmoZPhysics;
                bool highlightGizmoZ;

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

                void onViewportWidgetPostConstruct(WS2Editor::Widget::ViewportWidget &viewportWidget);
                void onViewportWidgetPreDestroy(WS2Editor::Widget::ViewportWidget &viewportWidget);

                void onViewportWidgetPostInitializeGl(WS2Editor::Widget::ViewportWidget &viewportWidget);

                void onViewportWidgetPhysicsObjectSelected(
                        btCollisionWorld::AllHitsRayResultCallback *rayCallback,
                        bool &outHandled
                        );

                void onViewportWidgetPhysicsObjectMouseOver(
                        btCollisionWorld::AllHitsRayResultCallback *rayCallback,
                        bool &outHandled
                        );

                void onViewportWidgetPhysicsObjectMouseOverNothing();

                void onRenderManagerPostInit(WS2Editor::RenderManager &renderManager);
                void onRenderManagerPreDestroy(WS2Editor::RenderManager &renderManager);

                /**
                 * @brief Updates the transformation gizmos and enqueues a draw command for it (If need be)
                 */
                void updateGizmos(WS2Editor::Resource::ResourceScene &scene);
        };
    }
}

#endif

