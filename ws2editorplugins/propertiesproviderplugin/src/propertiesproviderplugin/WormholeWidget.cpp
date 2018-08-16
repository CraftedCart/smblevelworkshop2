#include "propertiesproviderplugin/WormholeWidget.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include <QFontDatabase>
#include <QFontMetrics>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor;
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Editor::Project;
        using namespace WS2Common;
        using namespace WS2Common::Scene;

        WormholeWidget::WormholeWidget(QVector<WormholeSceneNode*> &nodes, QString label, QWidget *parent) :
            CollapsableWidget(label, 100, parent)
        {
            selectedWormholeNodes = nodes;

            QVBoxLayout *sectionLayout = new QVBoxLayout();
            sectionLayout->setMargin(0);
            setContentLayout(*sectionLayout);

            //Create widgets
            QHBoxLayout *destinationUuidLayout = new QHBoxLayout();
            sectionLayout->addLayout(destinationUuidLayout);

            destinationUuidLayout->addWidget(new QLabel(tr("Destination UUID")));

            //Destination UUID line edit
            destinationUuidLineEdit = new QLineEdit();
            destinationUuidLineEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
            destinationUuidLayout->addWidget(destinationUuidLineEdit);

            //Link selected button
            linkSelectedButton = new QPushButton(tr("Link selected wormholes"));
            sectionLayout->addWidget(linkSelectedButton);

            //Destination UUID alert
            destinationAlertWidget = new QWidget();
            destinationAlertWidget->setObjectName("destinationAlertWidget");
            QHBoxLayout *destinationAlertLayout = new QHBoxLayout();
            destinationAlertWidget->setLayout(destinationAlertLayout);
            sectionLayout->addWidget(destinationAlertWidget);

            alertIconLabel = new QLabel();
            QFontMetrics fm(QFontDatabase::systemFont(QFontDatabase::FixedFont));
            alertPixmap = QPixmap(":/Workshop2/Icons/Alert.png").scaledToHeight(fm.height() + 4, Qt::SmoothTransformation);
            alertIconLabel->setPixmap(alertPixmap);
            destinationAlertLayout->addWidget(alertIconLabel);

            alertTextLabel = new QLabel();
            alertTextLabel->setWordWrap(true);
            alertTextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            destinationAlertLayout->addWidget(alertTextLabel);

            //Destination UUID info
            destinationInfoWidget = new QWidget();
            destinationInfoWidget->setObjectName("destinationInfoWidget");
            QHBoxLayout *destinationInfoLayout = new QHBoxLayout();
            destinationInfoWidget->setLayout(destinationInfoLayout);
            sectionLayout->addWidget(destinationInfoWidget);

            infoIconLabel = new QLabel();
            wormholePixmap = QPixmap(":/Workshop2/Icons/Wormhole.png").scaledToHeight(fm.height() + 4, Qt::SmoothTransformation);
            infoIconLabel->setPixmap(wormholePixmap);
            destinationInfoLayout->addWidget(infoIconLabel);

            infoTextLabel = new QLabel();
            infoTextLabel->setWordWrap(true);
            infoTextLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
            destinationInfoLayout->addWidget(infoTextLabel);

            connect(destinationUuidLineEdit, &QLineEdit::editingFinished,
                    [this]() { onDestinationModifiedConfirmed(destinationUuidLineEdit->text()); });
            connect(destinationUuidLineEdit, &QLineEdit::textEdited, this, &WormholeWidget::onDestinationEdited);
            connect(linkSelectedButton, &QPushButton::pressed, this, &WormholeWidget::linkSelectedWormholes);

            updateValues();

            connect(ModelManager::modelOutliner, &ModelOutliner::onNodeModified, this, &WormholeWidget::onNodeModified);
        }

        void WormholeWidget::updateValues() {
            //Destination UUID
            if (selectedWormholeNodes.size() > 1) {
                //Check if 2 are selected and are linked together
                if (selectedWormholeNodes.size() == 2 &&
                        selectedWormholeNodes.at(0)->getDestinationUuid() == selectedWormholeNodes.at(1)->getUuid() &&
                        selectedWormholeNodes.at(1)->getDestinationUuid() == selectedWormholeNodes.at(0)->getUuid()) {
                    destinationUuidLineEdit->setText(tr("[Multiple values - linked]"));
                } else {
                    destinationUuidLineEdit->setText(tr("[Multiple values]"));
                }

                destinationUuidLineEdit->setReadOnly(true);
            } else {
                QString uuid = selectedWormholeNodes.at(0)->getDestinationUuid().toString();
                //Only set if it differs from the current value so we don't jump the cursor to the end while typing
                if (destinationUuidLineEdit->text() != uuid) destinationUuidLineEdit->setText(uuid);
                destinationUuidLineEdit->setReadOnly(false);
            }

            //Check if the link button should be shown
            linkSelectedButton->setVisible(selectedWormholeNodes.size() == 2);

            onDestinationEdited(destinationUuidLineEdit->text());
        }

        void WormholeWidget::onNodeModified(SceneNode *node) {
            if (selectedWormholeNodes.contains(dynamic_cast<WormholeSceneNode*>(node))) updateValues();
        }

        void WormholeWidget::onDestinationModifiedConfirmed(const QString &newDestinationUuid) {
            for (WormholeSceneNode *node : selectedWormholeNodes) {
                node->setDestinationUuid(newDestinationUuid);
                ModelManager::modelOutliner->onNodeModified(node);
            }
        }

        void WormholeWidget::onDestinationEdited(const QString &newDestinationUuid) {
            //Ignore [Multiple values]
            if (selectedWormholeNodes.size() > 1) {
                destinationAlertWidget->hide();
                destinationInfoWidget->hide();
                return;
            }

            QUuid uuid(newDestinationUuid);

            //Check if it's a valid UUID
            if (uuid.isNull()) {
                destinationAlertWidget->show();
                destinationInfoWidget->hide();
                alertTextLabel->setText(tr("Invalid UUID"));
            } else {
                SceneNode *node = ProjectManager::getActiveProject()->getScene()->getRootNode()->findNodeByUuid(uuid);

                if (node == nullptr) { //Check that the UUID exists
                    destinationAlertWidget->show();
                    destinationInfoWidget->hide();
                    alertTextLabel->setText(tr("UUID does not match any nodes in the scene"));
                } else if (dynamic_cast<WormholeSceneNode*>(node) == nullptr) { //Check that the linked object is a wormhole
                    destinationAlertWidget->show();
                    destinationInfoWidget->hide();
                    alertTextLabel->setText(tr("Destination node \"%1\" is not a wormhole").arg(node->getName()));
                } else {
                    destinationAlertWidget->hide();

                    infoTextLabel->setText(tr("Linked to \"%1\"").arg(node->getName()));
                    destinationInfoWidget->show();
                }
            }
        }

        void WormholeWidget::linkSelectedWormholes() {
            WormholeSceneNode *nodeA = selectedWormholeNodes.at(0);
            WormholeSceneNode *nodeB = selectedWormholeNodes.at(1);

            nodeA->setDestinationUuid(nodeB->getUuid());
            nodeB->setDestinationUuid(nodeA->getUuid());

            ModelManager::modelOutliner->onNodeModified(nodeA);
            ModelManager::modelOutliner->onNodeModified(nodeB);
        }
    }
}

