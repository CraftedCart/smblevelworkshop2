#include "propertiesproviderplugin/BananaWidget.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include <QLabel>
#include <QButtonGroup>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor;
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Editor::Project;
        using namespace WS2Common;
        using namespace WS2Common::Scene;

        BananaWidget::BananaWidget(QVector<BananaSceneNode*> &nodes, RenderManager *renderManager, QString label, QWidget *parent) :
            CollapsableWidget(label, 100, parent),
            renderManager(renderManager)
        {
            selectedBananaNodes = nodes;

            QVBoxLayout *sectionLayout = new QVBoxLayout();
            sectionLayout->setMargin(0);
            setContentLayout(*sectionLayout);

            //Create widgets
            sectionLayout->addWidget(new QLabel(tr("Type")));

            QButtonGroup *typeButtonGroup = new QButtonGroup(this);

            QHBoxLayout *typeLayout = new QHBoxLayout();
            sectionLayout->addLayout(typeLayout);

            singleTypeButton = new QPushButton(tr("Single"));
            singleTypeButton->setCheckable(true);
            typeLayout->addWidget(singleTypeButton);
            typeButtonGroup->addButton(singleTypeButton);

            bunchTypeButton = new QPushButton(tr("Bunch"));
            bunchTypeButton->setCheckable(true);
            typeLayout->addWidget(bunchTypeButton);
            typeButtonGroup->addButton(bunchTypeButton);

            connect(singleTypeButton, &QPushButton::clicked, [this]() { onTypeModified(EnumBananaType::SINGLE); });
            connect(bunchTypeButton, &QPushButton::clicked, [this]() { onTypeModified(EnumBananaType::BUNCH); });

            updateValues();

            connect(ModelManager::modelOutliner, &ModelOutliner::onNodeModified, this, &BananaWidget::onNodeModified);
        }

        void BananaWidget::updateValues() {
            EnumBananaType bananaType;
            bool hasSetBananaType = false;
            bool isSingleBananaType = true;

            for (BananaSceneNode *node : selectedBananaNodes) {
                if (hasSetBananaType) {
                    if (node->getType() != bananaType) {
                        isSingleBananaType = false;
                        break;
                    }
                } else {
                    bananaType = node->getType();
                    hasSetBananaType = true;
                }
            }

            singleTypeButton->setChecked(false);
            bunchTypeButton->setChecked(false);

            if (isSingleBananaType) {
                switch (bananaType) {
                    case SINGLE:
                        singleTypeButton->setChecked(true); break;
                    case BUNCH:
                        bunchTypeButton->setChecked(true); break;
                }
            }
        }

        void BananaWidget::onNodeModified(SceneNode *node) {
            if (selectedBananaNodes.contains(dynamic_cast<BananaSceneNode*>(node))) updateValues();
        }

        void BananaWidget::onTypeModified(EnumBananaType newType) {
            for (BananaSceneNode *node : selectedBananaNodes) {
                node->setType(newType);

                //We need to reconstruct the mesh data for the banana else it will remain using its old model
                ProjectManager::getActiveProject()->getScene()->removeMeshNodeData(node->getUuid());

                switch (newType) {
                    case SINGLE:
                        ProjectManager::getActiveProject()->getScene()->
                            addMeshNodeData(node->getUuid(), new MeshNodeData(node, renderManager->bananaSingleMesh));
                        break;
                    case BUNCH:
                        ProjectManager::getActiveProject()->getScene()->
                            addMeshNodeData(node->getUuid(), new MeshNodeData(node, renderManager->bananaBunchMesh));
                        break;
                }

                ModelManager::modelOutliner->onNodeModified(node);
            }
        }
    }
}

