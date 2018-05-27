/**
 * @file
 * @brief Header for the StageEditorWindow class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_MAINWINDOW_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_MAINWINDOW_HPP

#include "ws2editor_export.h"
#include "ws2editor/widget/ViewportWidget.hpp"
#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>

namespace Ui {
    class WS2EDITOR_EXPORT StageEditorWindow;
}

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT StageEditorWindow : public QMainWindow {
            Q_OBJECT

            public:
                QLabel *statusTaskLabel = new QLabel();
                QLabel *statusFramerateLabel = new QLabel();

            private:
                Ui::StageEditorWindow *ui;

            public:
                explicit StageEditorWindow(QWidget *parent = 0);
                ~StageEditorWindow();

            public slots:
                void viewportFrameRendered(qint64 deltaNanoseconds);

                /**
                 * @brief Shows a file chooser to import files. If files are picked, the files will be imported.
                 */
                void askImportFiles();

                /**
                 * @brief Adds an empty scene node to the currently active project
                 */
                void addSceneNode();

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

                void addGoalBlue();
                void addGoalGreen();
                void addGoalRed();
        };
    }
}

#endif

