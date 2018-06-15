#include "propertiesproviderplugin/GoalWidget.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include <QLabel>
#include <QButtonGroup>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Common::Scene;
        using namespace WS2Common;

        GoalWidget::GoalWidget(QVector<GoalSceneNode*> &nodes, QString label, QWidget *parent) :
            CollapsableWidget(label, 100, parent)
        {
            selectedGoalNodes = nodes;

            QVBoxLayout *sectionLayout = new QVBoxLayout();
            sectionLayout->setMargin(0);
            setContentLayout(*sectionLayout);

            //Create widgets
            sectionLayout->addWidget(new QLabel(tr("Type")));

            QButtonGroup *typeButtonGroup = new QButtonGroup(this);

            QHBoxLayout *typeLayout = new QHBoxLayout();
            sectionLayout->addLayout(typeLayout);

            blueTypeButton = new QPushButton(tr("Blue"));
            blueTypeButton->setCheckable(true);
            typeLayout->addWidget(blueTypeButton);
            typeButtonGroup->addButton(blueTypeButton);

            greenTypeButton = new QPushButton(tr("Green"));
            greenTypeButton->setCheckable(true);
            typeLayout->addWidget(greenTypeButton);
            typeButtonGroup->addButton(greenTypeButton);

            redTypeButton = new QPushButton(tr("Red"));
            redTypeButton->setCheckable(true);
            typeLayout->addWidget(redTypeButton);
            typeButtonGroup->addButton(redTypeButton);

            connect(blueTypeButton, &QPushButton::clicked, [this]() { onTypeModified(EnumGoalType::BLUE); });
            connect(greenTypeButton, &QPushButton::clicked, [this]() { onTypeModified(EnumGoalType::GREEN); });
            connect(redTypeButton, &QPushButton::clicked, [this]() { onTypeModified(EnumGoalType::RED); });

            updateValues();

            connect(ModelManager::modelOutliner, &ModelOutliner::onNodeModified, this, &GoalWidget::onNodeModified);
        }

        void GoalWidget::updateValues() {
            EnumGoalType goalType;
            bool hasSetGoalType = false;
            bool isSingleGoalType = true;

            for (GoalSceneNode *node : selectedGoalNodes) {
                if (hasSetGoalType) {
                    if (node->getType() != goalType) {
                        isSingleGoalType = false;
                        break;
                    }
                } else {
                    goalType = node->getType();
                    hasSetGoalType = true;
                }
            }

            blueTypeButton->setChecked(false);
            greenTypeButton->setChecked(false);
            redTypeButton->setChecked(false);

            if (isSingleGoalType) {
                switch (goalType) {
                    case BLUE:
                        blueTypeButton->setChecked(true); break;
                    case GREEN:
                        greenTypeButton->setChecked(true); break;
                    case RED:
                        redTypeButton->setChecked(true); break;
                }
            }
        }

        void GoalWidget::onNodeModified(SceneNode *node) {
            if (selectedGoalNodes.contains(dynamic_cast<GoalSceneNode*>(node))) updateValues();
        }

        void GoalWidget::onTypeModified(EnumGoalType newType) {
            for (GoalSceneNode *node : selectedGoalNodes) {
                node->setType(newType);
                ModelManager::modelOutliner->onNodeModified(node);
            }
        }
    }
}

