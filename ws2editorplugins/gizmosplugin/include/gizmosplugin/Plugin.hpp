/**
 * @file
 * @brief Example plugin header
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_PLUGIN_HPP
#define SMBLEVELWORKSHOP2_WS2EDITORPLUGINS_GIZMOSPLUGIN_PLUGIN_HPP

#include "ws2editor/plugin/IEditorPlugin.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include <QMouseEvent>

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

                /**
                 * @brief True if the gizmo is shown in the world and its physics objects (To check whether the mouse
                 *        is over it) are in the physics world
                 */
                bool isGizmoPhysicsInWorld = false;

                WS2Editor::Physics::PhysicsContainer *gizmoYPhysics;
                bool highlightGizmoY;
                bool activeGizmoY;
                WS2Editor::Physics::PhysicsContainer *gizmoXPhysics;
                bool highlightGizmoX;
                bool activeGizmoX;
                WS2Editor::Physics::PhysicsContainer *gizmoZPhysics;
                bool highlightGizmoZ;
                bool activeGizmoZ;

                /**
                 * @brief The location of the glzmos
                 */
                glm::vec3 gizmoPosition;

                /**
                 * @brief Where the mouse cursor was (In 3D space) upon beginning dragging of the gizmos
                 */
                glm::vec3 preGrabPosition;

                /**
                 * @brief The transforms of each topmost selected object before the user started dragging
                 */
                QHash<WS2Common::Scene::SceneNode*, WS2Common::Transform> preGrabTransforms;

            public:
                /**
                 * @brief The init function is called for each plugin while ws2editor is loading
                 *
                 * @return Whether this plugin initialized successfully or not
                 */
                virtual bool init() override;

                void onViewportWidgetMouseOverAnything();

            public slots:
                /**
                 * @brief Called after the StageEditorWindow has been constructed, so that the plugin can get a handle
                 *        on the window and hook into its functions/events there
                 *
                 * @param w The StageEditorWindow
                 */
                void onStageEditorWindowConstructed(WS2Editor::UI::StageEditorWindow &w);

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

                void onViewportWidgetMousePressed(QMouseEvent *event);
                void onViewportWidgetMouseReleased(QMouseEvent *event);

                void onViewportWidgetPostPreDraw(WS2Editor::Widget::ViewportWidget &viewportWidget);

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

