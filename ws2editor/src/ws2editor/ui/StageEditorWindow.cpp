#include "ws2editor/ui/StageEditorWindow.hpp"
#include "ws2editor/ui_StageEditorWindow.h"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/ui/SettingsDialog.hpp"
#include "ws2editor/ui/AboutWindow.hpp"
#include "ws2editor/ui/StageIdeaGeneratorWindow.hpp"
#include "ws2editor/ui/PluginsWindow.hpp"
#include "ws2editor/ui/CommandWidget.hpp"
#include "ws2editor/ui/StatusPopupWidget.hpp"
#include "ws2editor/task/ImportFileTask.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#include "ws2common/scene/GroupSceneNode.hpp"
#include "ws2common/scene/BackgroundGroupSceneNode.hpp"
#include "ws2common/scene/StartSceneNode.hpp"
#include "ws2common/scene/GoalSceneNode.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/WormholeSceneNode.hpp"
#include <QFontDatabase>
#include <QFileDialog>
#include <QAction>
#include <QDesktopServices>
#include <QDebug>
#include <Qt>

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
            connect(ui->actionExport, &QAction::triggered, this, &StageEditorWindow::askExportFilesProvider);
            connect(ui->actionRunCommand, &QAction::triggered, this, &StageEditorWindow::showCommandLine);
            connect(ui->actionNewNode, &QAction::triggered, this, &StageEditorWindow::addSceneNode);
            connect(ui->actionNewBackgroundGroupNode, &QAction::triggered, this, &StageEditorWindow::addBackgroundNode);
            connect(ui->actionDropToGround, &QAction::triggered, this, &StageEditorWindow::dropToGroundSelected);
            connect(ui->actionDelete, &QAction::triggered, this, &StageEditorWindow::deleteSelected);
            connect(ui->actionSettings, &QAction::triggered, this, &StageEditorWindow::showSettings);
            connect(ui->actionAbout, &QAction::triggered, this, &StageEditorWindow::showAbout);
            connect(ui->actionStageIdeaGenerator, &QAction::triggered, this, &StageEditorWindow::showStageIdeaGenerator);
            connect(ui->actionViewPlugins, &QAction::triggered, this, &StageEditorWindow::showPlugins);
            connect(ui->actionWorkshopDiscord, &QAction::triggered, []() { QDesktopServices::openUrl(QUrl("https://discord.gg/CEYjvDj")); });
            connect(ui->actionAddStart, &QAction::triggered, this, &StageEditorWindow::addStart);
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

            //Install an event filter for the viewport
            //so we don't trigger shortcuts when a mouse button is down
            ui->viewportWidget->installEventFilter(new ViewportEventFilter(this, this));

            //Disable not yet implemented functionality
            ui->actionNew->setEnabled(false);
            ui->actionOpen->setEnabled(false);
            ui->actionUndo->setEnabled(false);
            ui->actionRedo->setEnabled(false);
            ui->actionCut->setEnabled(false);
            ui->actionCopy->setEnabled(false);
            ui->actionPaste->setEnabled(false);
            ui->actionSelectAll->setEnabled(false);
            ui->actionDuplicate->setEnabled(false);
            ui->actionFocusSelected->setEnabled(false);
            ui->actionSave->setEnabled(false);
            ui->actionSaveAs->setEnabled(false);

            //Hide NYI dockables
            ui->timelineDockWidget->hide();
            ui->actionToggleViewTimeline->setEnabled(false);
        }

        StageEditorWindow::~StageEditorWindow() {
            delete statusFramerateLabel;
            delete ui;
        }

        void StageEditorWindow::checkShortcutsEnabled() {
            QList<QAction*> actions = findChildren<QAction*>();

            if (QApplication::mouseButtons() != Qt::NoButton) {
                for (QAction *a : actions) a->setShortcutContext(Qt::WidgetShortcut);
            } else if (QApplication::keyboardModifiers() == Qt::NoModifier) {
                //Don't re-enable shortcuts until modifers have been released
                for (QAction *a : actions) a->setShortcutContext(Qt::WindowShortcut);
            }
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

        void StageEditorWindow::askExportFilesProvider() {
            QMenu m(tr("Export menu"), this);

            //Add a title to the menu
            QAction *titleAction = new QAction(tr("Export providers"), &m);
            titleAction->setEnabled(false);
            m.addAction(titleAction);

            //Fetch export providers
            QVector<IExportProvider*> &exportProviders = WS2EditorInstance::getInstance()->getExportProviders();

            if (exportProviders.isEmpty()) {
                //No providers
                QAction *a = new QAction(tr("No export providers"), &m);
                a->setEnabled(false);
                m.addAction(a);
            } else {
                //Set up provider actions
                for (IExportProvider *provider : exportProviders) {
                    QAction *a = new QAction(provider->getTranslatedTypeName(), &m);
                    connect(a, &QAction::triggered, [this, provider]() { askExportFiles(provider); });
                    m.addAction(a);
                }
            }

            m.exec(QCursor::pos());
        }

        void StageEditorWindow::askExportFiles(IExportProvider *provider) {
            //First check if the export provider will allow us
            Result<void, QString> r = provider->checkProject(ProjectManager::getActiveProject());

            if (r.isErr()) {
                QString message = r.unwrapErr();
                if (message.isEmpty()) message = tr("Project check failed - no message provided");

                StatusPopupWidget *w = new StatusPopupWidget(QCursor::pos(), message, "statusMessageFailed", this);
                w->show();

                return;
            }

            QFileDialog dialog(this);
            dialog.setFileMode(QFileDialog::AnyFile);

            //Set name filters
            QStringList l;
            for (QPair<QString, QString> p : provider->getNameFilters()) l << p.first;
            l << tr("All files (*)");
            dialog.setNameFilters(l);

            //Set the default extension if there is one
            if (!provider->getNameFilters().isEmpty()) dialog.selectFile(provider->getNameFilters().at(0).second);

            if (dialog.exec()) {
                QStringList selected = dialog.selectedFiles();

                //Check the selected filter and append the appropriate file extension if it's not already there
                QString selectedFilter = dialog.selectedNameFilter();
                for (QPair<QString, QString> p : provider->getNameFilters()) {
                    if (p.first == selectedFilter) {
                        //Found a matching filter
                        //Append the appropriate extension if it's not already there
                        for (QString &file : selected) if (!file.endsWith(p.second)) file += p.second;

                        break;
                    }
                }

                //We now have our final file name list
                //Let the export provider take over now
                provider->exportFiles(selected, ProjectManager::getActiveProject());
            }
        }

        void StageEditorWindow::addSceneNode() {
            GroupSceneNode *newNode = new GroupSceneNode(tr("New Item Group"));
            ModelManager::modelOutliner->addNode(newNode, ProjectManager::getActiveProject()->getScene()->getRootNode());
        }

        void StageEditorWindow::addBackgroundNode() {
            BackgroundGroupSceneNode *newNode = new BackgroundGroupSceneNode(tr("New Background Group"));
            ModelManager::modelOutliner->addNode(newNode, ProjectManager::getActiveProject()->getScene()->getRootNode());
        }

        void StageEditorWindow::dropToGroundSelected() {
            QVector<SceneNode*> selected = ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getSelectedObjects();

            for (SceneNode *node : selected) {
                btCollisionWorld::AllHitsRayResultCallback *rayCallback = raycastWorld(
                        node->getPosition(),
                        node->getPosition() + glm::vec3(0.0f, -1000.0f, 0.0f)
                        );

                if (rayCallback->hasHit()) {
                    //Fetch the closest hit
                    QVector<int> indices = sortAllHitsRayResultCallback(rayCallback);
                    glm::vec3 hitPoint = MathUtils::toGlmVec3(rayCallback->m_hitPointWorld[indices.at(0)]);
                    glm::vec3 hitNorm = MathUtils::toGlmVec3(rayCallback->m_hitNormalWorld[indices.at(0)]);

                    //Some objects require an offset
                    if (dynamic_cast<BananaSceneNode*>(node) || dynamic_cast<StartSceneNode*>(node)) {
                        hitPoint += 0.5f * hitNorm;
                    }

                    node->setPosition(hitPoint);
                    // node->setRotation(hitNorm);
                    ModelManager::modelOutliner->nodeModified(node);
                }
            }
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

        void StageEditorWindow::addNodeToRoot(SceneNode *node, QVector<ResourceMesh*>& meshes) {
            ResourceScene *scene = ProjectManager::getActiveProject()->getScene();
            SceneNode *rootNode = scene->getRootNode();

            ModelManager::modelOutliner->addNodeWithMeshes(node, rootNode, meshes);

            //Select the node
            ProjectManager::getActiveProject()->getScene()->getSelectionManager()->selectOnly(node);
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

        btCollisionWorld::AllHitsRayResultCallback* StageEditorWindow::raycastWorld(glm::vec3 startPos, glm::vec3 endPos) {
            btCollisionWorld::AllHitsRayResultCallback *rayCallback = new btCollisionWorld::AllHitsRayResultCallback(
                    btVector3(WS2Common::MathUtils::toBtVector3(startPos)),
                    btVector3(WS2Common::MathUtils::toBtVector3(endPos))
                    );
            rayCallback->m_flags = btTriangleRaycastCallback::kF_FilterBackfaces; //Ignore back faces

            WS2Editor::Project::ProjectManager::getActiveProject()->getScene()->getPhysicsManager()->getDynamicsWorld()->rayTest(
                    btVector3(WS2Common::MathUtils::toBtVector3(startPos)),
                    btVector3(WS2Common::MathUtils::toBtVector3(endPos)),
                    *rayCallback
                    );


            if (rayCallback->hasHit()) {
                for (int i = 0; i < rayCallback->m_collisionObjects.size(); i++) {
                    btVector3 hitPoint = rayCallback->m_hitPointWorld[i];
                    btVector3 hitNorm = rayCallback->m_hitNormalWorld[i];

                    WS2Editor::Project::ProjectManager::getActiveProject()->getScene()->getPhysicsDebugDrawer()->drawLine(
                            hitPoint,
                            hitPoint + hitNorm * 10.0f,
                            btVector3(1.0f, 0.0f, 1.0f)
                            );
                }
            }

            return rayCallback;
        }

        QVector<int> StageEditorWindow::sortAllHitsRayResultCallback(const btCollisionWorld::AllHitsRayResultCallback *rayCallback) {
            //Create an indices array and fill it incrementally(0, 1, 2, 3, etc. for as many hit objects as there are)
            QVector<int> indices(rayCallback->m_hitFractions.size());
            std::iota(indices.begin(), indices.end(), 0);

            //Sort the indices array based on the fractions
            class SortIndices {
                private:
                    const btScalar *arr;

                public:
                    SortIndices(const btScalar *arr) : arr(arr) {}
                    bool operator()(int a, int b) const { return arr[a] < arr[b]; }
            };
            std::sort(indices.begin(), indices.end(), SortIndices(&rayCallback->m_hitFractions[0]));

            return indices;
        }

        void StageEditorWindow::addStart() {
            StartSceneNode *newNode = new StartSceneNode(tr("Player Start"));
            addNodeToRoot(newNode, ui->viewportWidget->getRenderManager()->playerBallMesh);
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

        //==============================================================================================================

        ViewportEventFilter::ViewportEventFilter(StageEditorWindow *w, QObject *parent) :
            QObject(parent),
            w(w) {}

        bool ViewportEventFilter::eventFilter(QObject *watched, QEvent *event) {
            if (event->type() == QEvent::MouseButtonPress ||
                    event->type() == QEvent::MouseButtonRelease ||
                    event->type() == QEvent::KeyRelease) {
                w->checkShortcutsEnabled();
            }

            return QObject::eventFilter(watched, event);
        }
    }
}

