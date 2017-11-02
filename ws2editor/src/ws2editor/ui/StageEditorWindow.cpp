#include "ws2editor/ui/StageEditorWindow.hpp"
#include "ws2editor/ui_StageEditorWindow.h"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/ui/SettingsDialog.hpp"
#include "ws2editor/ui/AboutWindow.hpp"
#include "ws2editor/ui/StageIdeaGeneratorWindow.hpp"
#include "ws2editor/task/ImportFileTask.hpp"
#include "ws2editor/WS2.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include <QFontDatabase>
#include <Qt>
#include <QFileDialog>
#include <QAction>
#include <QDebug>

namespace WS2Editor {
    namespace UI {
        StageEditorWindow::StageEditorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StageEditorWindow) {
            ui->setupUi(this);

            statusTaskProgressBar->hide();
            statusTaskProgressBar->setMinimumWidth(128); //Prevent the progress bar size from fluctuating
            statusTaskProgressBar->setMaximumWidth(128);
            ui->statusBar->addWidget(statusTaskProgressBar);
            connect(ws2TaskManager->getProgress(), &Progress::valueChanged, statusTaskProgressBar, &QProgressBar::setValue);
            connect(ws2TaskManager->getProgress(), &Progress::maxChanged, [=](unsigned int max) {
                        if (max == 0) {
                            statusTaskProgressBar->hide();
                        } else {
                            statusTaskProgressBar->setMaximum(max);
                            statusTaskProgressBar->show();
                        }
                    });

            ui->statusBar->addWidget(statusTaskLabel);
            connect(ws2TaskManager, &Task::TaskManager::messageChanged, statusTaskLabel, &QLabel::setText);

            const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
            statusFramerateLabel->setFont(fixedFont);
            statusFramerateLabel->setAutoFillBackground(true); //Allow changing the background color
            ui->statusBar->addPermanentWidget(statusFramerateLabel); //addPermanentWidget right aligns this

            ui->resourcesTableView->setModel(UI::ModelManager::modelResources);
            ui->outlinerTreeView->setModel(UI::ModelManager::modelOutliner);

            //If the selection is changed, update the outliner tree view
            connect(UI::ModelManager::modelOutliner, &UI::ModelOutliner::onSelectionChanged,
                    ui->outlinerTreeView, &Widget::OutlinerWidget::onSelectionChanged);

            //Hide the following dock widgets on start
            ui->resourcesDockWidget->hide();

            connect(ui->viewportWidget, &Widget::ViewportWidget::frameRendered, this, &StageEditorWindow::viewportFrameRendered);
            connect(ui->actionQuit, &QAction::triggered, QApplication::instance(), QApplication::quit);
            connect(ui->actionImport, &QAction::triggered, this, &StageEditorWindow::askImportFiles);
            connect(ui->actionNewNode, &QAction::triggered, this, &StageEditorWindow::addSceneNode);
            connect(ui->actionSettings, &QAction::triggered, this, &StageEditorWindow::showSettings);
            connect(ui->actionAbout, &QAction::triggered, this, &StageEditorWindow::showAbout);
            connect(ui->actionStageIdeaGenerator, &QAction::triggered, this, &StageEditorWindow::showStageIdeaGenerator);
        }

        StageEditorWindow::~StageEditorWindow() {
            delete statusFramerateLabel;
            delete ui;
        }

        void StageEditorWindow::viewportFrameRendered(qint64 deltaNanoseconds) {
            float deltaMs = deltaNanoseconds / 1000000.0f;
            float fps = 1000000000.0f / deltaNanoseconds;

            statusFramerateLabel->setText(QString(tr("Delta: %1ms / Framerate: %2")).arg(
                        QString::number(deltaMs, 'f', 2),
                        QString::number(fps, 'f', 2)
                        ));
        }

        void StageEditorWindow::askImportFiles() {
            QList<QUrl> urls = QFileDialog::getOpenFileUrls(
                    this,
                    tr("Import files"),
                    QUrl(),
                    tr("All supported file types (*.fbx *.dae *.gltf *.glb *.blend *.3ds *.ase *.obj *.ifc *.xgl "
                        "*.zgl *.ply *.dxf *.lwo *.lws *.lxo *.stl *.x *.ac *ms3d *.cob *.scn *.mesh.xml *.irrmesh *.irr "
                        "*.mdl *.md2 *.md3 *.pk3 *.mdc *.md5* *.smd *.vta *.ogex *.3d *.b3d *.q3d *.q3s *.nff *.off *.raw "
                        "*.ter *.hmp *.ndo *.txt *.xml);;"

                        "3D models (*.fbx *.dae *.gltf *.glb *.blend *.3ds *.ase *.obj *.ifc *.xgl *.zgl *.ply *.dxf *.lwo "
                        "*.lws *.lxo *.stl *.x *.ac *ms3d *.cob *.scn *.mesh.xml *.irrmesh *.irr *.mdl *.md2 *.md3 *.pk3 "
                        "*.mdc *.md5* *.smd *.vta *.ogex *.3d *.b3d *.q3d *.q3s *.nff *.off *.raw *.ter *.hmp *.ndo);;"

                        "Stage configurations (*.txt *.xml);;"

                        "All files (*)")
                    );

            //Get outta here if no files were chosen
            if (urls.isEmpty()) return;

            QVector<Task::Task*> tasks;

            for (int i = 0; i < urls.size(); i++) {
                QFile *f = new QFile(urls.at(i).toLocalFile());
                std::function<void()> *func = new std::function<void()>([=]() {ui->viewportWidget->makeCurrentContext();});
                tasks.append(new Task::ImportFileTask(f, func));
            }

            ws2TaskManager->enqueueTasks(tasks);
        }

        void StageEditorWindow::addSceneNode() {
            WS2Common::Scene::GroupSceneNode *newNode = new WS2Common::Scene::GroupSceneNode(tr("New Node"));
            Project::ProjectManager::getActiveProject()->getScene()->getRootNode()->addChild(newNode);
            ModelManager::modelOutliner->onNodeAdded(newNode); //TODO: This feels hacky
        }

        void StageEditorWindow::showSettings() {
            SettingsDialog *win = new SettingsDialog();
            win->show();
        }

        void StageEditorWindow::showAbout() {
            AboutWindow *win = new AboutWindow();
            win->show();
        }

        void StageEditorWindow::showStageIdeaGenerator() {
            StageIdeaGeneratorWindow *win = new StageIdeaGeneratorWindow();
            win->show();
        }
    }
}

