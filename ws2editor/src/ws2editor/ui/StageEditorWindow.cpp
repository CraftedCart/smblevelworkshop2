#include "ws2editor/ui/StageEditorWindow.hpp"
#include "ws2editor/ui_StageEditorWindow.h"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/ui/SettingsDialog.hpp"
#include "ws2editor/ui/AboutWindow.hpp"
#include "ws2editor/ui/StageIdeaGeneratorWindow.hpp"
#include "ws2editor/ui/PluginsWindow.hpp"
#include "ws2editor/ui/CommandWidget.hpp"
#include "ws2editor/task/ImportFileTask.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/WormholeSceneNode.hpp"
#include <QFontDatabase>
#include <Qt>
#include <QFileDialog>
#include <QAction>
#include <QDesktopServices>
#include <QDebug>

namespace WS2Editor {
    namespace UI {
        using namespace WS2Editor::Resource;
        using namespace WS2Editor::Project;
        using namespace WS2Common::Scene;
        using namespace WS2Common::Resource;
        using namespace WS2Common;

        StageEditorWindow::StageEditorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StageEditorWindow) {
            ui->setupUi(this);

            setDockOptions(
                    QMainWindow::AnimatedDocks |
                    QMainWindow::AllowNestedDocks |
                    QMainWindow::AllowTabbedDocks |
                    QMainWindow::GroupedDragging
                    );

            ui->statusBar->addWidget(statusTaskLabel);
            connect(WS2EditorInstance::getInstance()->getTaskManager(), &Task::TaskManager::messageChanged, statusTaskLabel, &QLabel::setText);

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
            connect(ui->actionRunCommand, &QAction::triggered, this, &StageEditorWindow::showCommandLine);
            connect(ui->actionNewNode, &QAction::triggered, this, &StageEditorWindow::addSceneNode);
            connect(ui->actionDelete, &QAction::triggered, this, &StageEditorWindow::deleteSelected);
            connect(ui->actionSettings, &QAction::triggered, this, &StageEditorWindow::showSettings);
            connect(ui->actionAbout, &QAction::triggered, this, &StageEditorWindow::showAbout);
            connect(ui->actionStageIdeaGenerator, &QAction::triggered, this, &StageEditorWindow::showStageIdeaGenerator);
            connect(ui->actionViewPlugins, &QAction::triggered, this, &StageEditorWindow::showPlugins);
            connect(ui->actionWorkshopDiscord, &QAction::triggered, []() { QDesktopServices::openUrl(QUrl("https://discord.gg/CEYjvDj")); });
            connect(ui->actionAddGoalBlue, &QAction::triggered, this, &StageEditorWindow::addGoalBlue);
            connect(ui->actionAddGoalGreen, &QAction::triggered, this, &StageEditorWindow::addGoalGreen);
            connect(ui->actionAddGoalRed, &QAction::triggered, this, &StageEditorWindow::addGoalRed);
            connect(ui->actionAddBumper, &QAction::triggered, this, &StageEditorWindow::addBumper);
            connect(ui->actionAddBanana, &QAction::triggered, this, &StageEditorWindow::addBananaSingle);
            connect(ui->actionAddBananaBunch, &QAction::triggered, this, &StageEditorWindow::addBananaBunch);
            connect(ui->actionAddJamabar, &QAction::triggered, this, &StageEditorWindow::addJamabar);
            connect(ui->actionAddWormhole, &QAction::triggered, this, &StageEditorWindow::addWormhole);

            //Debug menu
            connect(ui->actionClearAllRenderManagerCaches, &QAction::triggered, [this]() {
                    ui->viewportWidget->makeCurrentContext();
                    ui->viewportWidget->getRenderManager()->clearAllCaches();
                    });

            connect(ui->actionClearRenderManagerMeshCache, &QAction::triggered, [this]() {
                    ui->viewportWidget->makeCurrentContext();
                    ui->viewportWidget->getRenderManager()->clearMeshCache();
                    });

            connect(ui->actionClearRenderManagerTextureCache, &QAction::triggered, [this]() {
                    ui->viewportWidget->makeCurrentContext();
                    ui->viewportWidget->getRenderManager()->clearTextureCache();
                    });

            //Properties panel
            //TODO: This won't survive project/scene changes
            connect(ProjectManager::getActiveProject()->getScene()->getSelectionManager(), &Scene::SceneSelectionManager::onSelectionChanged,
                    ui->propertiesWidget, &Widget::PropertiesWidget::updatePropertiesWidget);
        }

        StageEditorWindow::~StageEditorWindow() {
            delete statusFramerateLabel;
            delete ui;
        }

        Widget::ViewportWidget* StageEditorWindow::getViewportWidget() {
            return ui->viewportWidget;
        }

        Widget::PropertiesWidget* StageEditorWindow::getPropertiesWidget() {
            return ui->propertiesWidget;
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
                tasks.append(new Task::ImportFileTask(f));
            }

            WS2EditorInstance::getInstance()->getTaskManager()->enqueueTasks(tasks);
        }

        void StageEditorWindow::addSceneNode() {
            GroupSceneNode *newNode = new GroupSceneNode(tr("New Node"));
            ModelManager::modelOutliner->addNode(newNode, ProjectManager::getActiveProject()->getScene()->getRootNode());
        }

        void StageEditorWindow::deleteSelected() {
            QVector<SceneNode*> toDelete = ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getSelectedObjects();

            for (SceneNode *node : toDelete) {
                ModelManager::modelOutliner->removeNode(node);
            }
        }

        void StageEditorWindow::showSettings() {
            SettingsDialog *win = new SettingsDialog(this);
            win->show();
        }

        void StageEditorWindow::showAbout() {
            AboutWindow *win = new AboutWindow(this);
            win->show();
        }

        void StageEditorWindow::showStageIdeaGenerator() {
            StageIdeaGeneratorWindow *win = new StageIdeaGeneratorWindow(this);
            win->show();
        }

        void StageEditorWindow::showPlugins() {
            PluginsWindow *win = new PluginsWindow(this);
            win->show();
        }

        void StageEditorWindow::showCommandLine() {
            CommandWidget *widget = new CommandWidget(QCursor::pos(), this);
            widget->show();
        }

        void StageEditorWindow::addNodeToStaticGroup(SceneNode *node, QVector<ResourceMesh*>& meshes) {
            ResourceScene *scene = ProjectManager::getActiveProject()->getScene();
            SceneNode *staticNode = scene->getStaticNode();

            if (staticNode == nullptr) {
                //No static nodes exist - create one
                staticNode = new WS2Common::Scene::GroupSceneNode(tr("Static"));
                ModelManager::modelOutliner->addNode(node, ProjectManager::getActiveProject()->getScene()->getRootNode());
            }

            ModelManager::modelOutliner->addNodeWithMeshes(node, staticNode, meshes);

            //Select the node
            ProjectManager::getActiveProject()->getScene()->getSelectionManager()->selectOnly(node);
        }

        void StageEditorWindow::addGoalBlue() {
            GoalSceneNode *newNode = new GoalSceneNode(tr("New Blue Goal"));
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->goalMesh);
        }

        void StageEditorWindow::addGoalGreen() {
            GoalSceneNode *newNode = new GoalSceneNode(tr("New Green Goal"));
            newNode->setType(EnumGoalType::GREEN);
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->goalMesh);
        }

        void StageEditorWindow::addGoalRed() {
            GoalSceneNode *newNode = new GoalSceneNode(tr("New Red Goal"));
            newNode->setType(EnumGoalType::RED);
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->goalMesh);
        }

        void StageEditorWindow::addBumper() {
            BumperSceneNode *newNode = new BumperSceneNode(tr("New Bumper"));
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->bumperMesh);
        }

        void StageEditorWindow::addBananaSingle() {
            BananaSceneNode *newNode = new BananaSceneNode(tr("New Banana Single"));
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->bananaSingleMesh);
        }

        void StageEditorWindow::addBananaBunch() {
            BananaSceneNode *newNode = new BananaSceneNode(tr("New Banana Bunch"));
            newNode->setType(EnumBananaType::BUNCH);
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->bananaBunchMesh);
        }

        void StageEditorWindow::addJamabar() {
            JamabarSceneNode *newNode = new JamabarSceneNode(tr("New Jamabar"));
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->jamabarMesh);
        }

        void StageEditorWindow::addWormhole() {
            WormholeSceneNode *newNode = new WormholeSceneNode(tr("New Wormhole"));
            addNodeToStaticGroup(newNode, ui->viewportWidget->getRenderManager()->wormholeMesh);
        }
    }
}

