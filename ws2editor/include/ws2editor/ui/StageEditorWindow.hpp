/**
 * @file
 * @brief Header for the StageEditorWindow class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_MAINWINDOW_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_MAINWINDOW_HPP

#include "ws2editor_export.h"
#include "ws2editor/widget/ViewportWidget.hpp"
#include "ws2editor/widget/PropertiesWidget.hpp"
#include "ws2editor/IExportProvider.hpp"
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <QMainWindow>
#include <QLabel>

namespace Ui {
    class WS2EDITOR_EXPORT StageEditorWindow;
}

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT StageEditorWindow : public QMainWindow {
            Q_OBJECT

            friend class ViewportEventFilter;

            private:
                Ui::StageEditorWindow *ui;

            public:
                QLabel *statusTaskLabel = new QLabel();
                QLabel *statusFramerateLabel = new QLabel();

            protected:
                /**
                 * @brief Checks if any mouse buttons are down and disables/enables shortcuts appropriately
                 */
                void checkShortcutsEnabled();

                void addNodeToRoot(
                        WS2Common::Scene::SceneNode *node,
                        QVector<WS2Common::Resource::ResourceMesh*>& meshes
                        );

                void addNodeToStaticGroup(
                        WS2Common::Scene::SceneNode *node,
                        QVector<WS2Common::Resource::ResourceMesh*>& meshes
                        );

                btCollisionWorld::AllHitsRayResultCallback* raycastWorld(glm::vec3 startPos, glm::vec3 endPos);

                /**
                 * @brief Sorts the ray callback result values by each hit point's distance from sourcePoint and returns an indices vector
                 *
                 * Elements are not directly sorted in the rayCallback. Rather, an indices vector is created, sorted based on the fractions in the ray callback, and returned
                 *
                 * @todo Move into a helper namespace instead of duplicating it in ViewportWidget
                 *
                 * @param rayCallback The ray callback result to sort the returned indices vector
                 *
                 * @return An indices array sorted based on distance
                 */
                QVector<int> sortAllHitsRayResultCallback(const btCollisionWorld::AllHitsRayResultCallback *rayCallback);

            public:
                explicit StageEditorWindow(QWidget *parent = 0);
                ~StageEditorWindow();

                /**
                 * @brief Returns the viewport widget in the stage editor
                 * @return The viewport widget
                 */
                Widget::ViewportWidget* getViewportWidget();

                /**
                 * @brief Returns the properties widget in the stage editor
                 * @return The properties widget
                 */
                Widget::PropertiesWidget* getPropertiesWidget();

            public slots:
                void viewportFrameRendered(qint64 deltaNanoseconds);

                /**
                 * @brief Shows a file chooser to import files. If files are picked, the files will be imported.
                 */
                void askImportFiles();

                /**
                 * @brief Shows a pop up menu to select an export provider
                 */
                void askExportFilesProvider();

                /**
                 * @brief Shows a file chooser to export files using the given export provider
                 *
                 * @param provider The export provider to use when generating data to write
                 */
                void askExportFiles(IExportProvider *provider);

                /**
                 * @brief Adds an empty group scene node to the currently active project
                 */
                void addSceneNode();

                /**
                 * @brief Adds an empty background scene node to the currently active project
                 */
                void addBackgroundNode();

                /**
                 * @brief Drops selected nodes to the ground below them
                 */
                void dropToGroundSelected();

                /**
                 * @brief Deletes selected nodes
                 */
                void deleteSelected();

                /**
                 * @brief Shows the settings dialog
                 */
                void showSettings();

                /**
                 * @brief Shows the about dialog
                 */
                void showAbout();

                /**
                 * @brief Shows the stage idea generator dialog
                 */
                void showStageIdeaGenerator();

                /**
                 * @brief Shows the plugins dialog
                 */
                void showPlugins();

                /**
                 * @brief Shows the command line widget as a pop-up
                 */
                void showCommandLine();

                void addStart();
                void addGoalBlue();
                void addGoalGreen();
                void addGoalRed();
                void addBumper();
                void addBananaSingle();
                void addBananaBunch();
                void addJamabar();
                void addWormhole();
        };

        /**
         * @brief Used to check if the mouse is pressed over the viewport and disable shortcuts if so
         */
        class WS2EDITOR_EXPORT ViewportEventFilter : public QObject {
            Q_OBJECT

            protected:
                StageEditorWindow *w;

            public:
                ViewportEventFilter(StageEditorWindow *w, QObject *parent = nullptr);

            protected:
                bool eventFilter(QObject *watched, QEvent *event);
        };
    }
}

#endif

