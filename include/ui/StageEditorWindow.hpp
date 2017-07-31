/**
 * @file
 * @brief Header for the StageEditorWindow class
 */

#ifndef SMBLEVELWORKSHOP2_UI_MAINWINDOW_HPP
#define SMBLEVELWORKSHOP2_UI_MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>

namespace Ui {
    class StageEditorWindow;
}

namespace WS2 {
    namespace UI {
        class StageEditorWindow : public QMainWindow {
            Q_OBJECT

            public:
                explicit StageEditorWindow(QWidget *parent = 0);
                ~StageEditorWindow();

                QLabel *statusFramerateLabel = new QLabel();

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

            private:
                Ui::StageEditorWindow *ui;
        };
    }
}

#endif

