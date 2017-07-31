#include "ui/StageEditorWindow.hpp"
#include "ui_StageEditorWindow.h"
#include "project/ProjectManager.hpp"
#include "ui/ModelManager.hpp"
#include <QFontDatabase>
#include <Qt>
#include <QFileDialog>
#include <QAction>

namespace WS2 {
    namespace UI {
        StageEditorWindow::StageEditorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StageEditorWindow) {
            ui->setupUi(this);

            const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
            statusFramerateLabel->setFont(fixedFont);
            statusFramerateLabel->setAutoFillBackground(true); //Allow changing the background color
            ui->statusBar->addWidget(statusFramerateLabel);

            ui->resourcesTableView->setModel(UI::ModelManager::modelResources);
            ui->outlinerTreeView->setModel(UI::ModelManager::modelOutliner);

            //If the selection is changed, update the outliner tree view
            connect(UI::ModelManager::modelOutliner, &UI::ModelOutliner::onSelectionChanged,
                    ui->outlinerTreeView, &Widget::OutlinerWidget::selectionChanged);

            //Hide the following dock widgets on start
            ui->resourcesDockWidget->hide();

            connect(ui->actionQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
            connect(ui->viewportWidget, &Widget::ViewportWidget::frameRendered, this, &StageEditorWindow::viewportFrameRendered);
            connect(ui->actionImport, &QAction::triggered, this, &StageEditorWindow::askImportFiles);
            connect(ui->actionNewNode, &QAction::triggered, this, &StageEditorWindow::addSceneNode);
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

            ui->viewportWidget->makeCurrentContext();

            for (int i = 0; i < urls.size(); i++) {
                QFile f(urls.at(i).toLocalFile());
                Project::ProjectManager::getActiveProject()->importFile(f);
            }
        }

        void StageEditorWindow::addSceneNode() {
            Project::ProjectManager::getActiveProject()->getScene()->getRootNode()->addChild(new Scene::SceneNode(tr("New Node")));
        }
    }
}

