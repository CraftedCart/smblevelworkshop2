#include "propertiesproviderplugin/NodeWidget.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2common/scene/BumperSceneNode.hpp"
#include "ws2common/scene/JamabarSceneNode.hpp"
#include "ws2common/scene/BananaSceneNode.hpp"
#include "ws2common/scene/FalloutVolumeSceneNode.hpp"
#include "ws2common/scene/MeshSceneNode.hpp" //TODO: Remove this include when we have a GX exporter, probably
#include <QFontDatabase>
#include <QLabel>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Editor::Project;
        using namespace WS2Common::Scene;

        NodeWidget::NodeWidget(QVector<SceneNode*> &nodes, QString label, QWidget *parent) :
            CollapsableWidget(label, 100, parent)
        {
            selectedNodes = nodes;

            QVBoxLayout *sectionLayout = new QVBoxLayout();
            sectionLayout->setMargin(0);
            setContentLayout(*sectionLayout);

            //Create widgets
            QHBoxLayout *nameLayout = new QHBoxLayout();
            sectionLayout->addLayout(nameLayout);
            nameLayout->addWidget(new QLabel(tr("Name")));
            nameLineEdit = new QLineEdit();
            nameLayout->addWidget(nameLineEdit);

            QHBoxLayout *uuidLayout = new QHBoxLayout();
            sectionLayout->addLayout(uuidLayout);
            uuidLayout->addWidget(new QLabel(tr("UUID")));
            uuidLineEdit = new QLineEdit();
            uuidLineEdit->setReadOnly(true);
            uuidLineEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
            uuidLayout->addWidget(uuidLineEdit);

            sectionLayout->addWidget(new QLabel(tr("Position")));
            posSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(posSpinBoxes);

            sectionLayout->addWidget(new QLabel(tr("Rotation")));
            rotSpinBoxes = new Vec3DraggableSpinBoxes();
            rotSpinBoxes->setSuffix(tr("°"));
            sectionLayout->addWidget(rotSpinBoxes);

            sectionLayout->addWidget(new QLabel(tr("Scale")));
            sclSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(sclSpinBoxes);

            updateValues();

            connect(ModelManager::modelOutliner, &ModelOutliner::onNodeModified, this, &NodeWidget::onNodeModified);

            connect(nameLineEdit, &QLineEdit::textEdited, this, &NodeWidget::onNameModified);
            connect(posSpinBoxes, &Vec3DraggableSpinBoxes::valueChanged, this, &NodeWidget::onPosModified);
            connect(rotSpinBoxes, &Vec3DraggableSpinBoxes::valueChanged, this, &NodeWidget::onRotModified);
            connect(sclSpinBoxes, &Vec3DraggableSpinBoxes::valueChanged, this, &NodeWidget::onSclModified);
        }

        void NodeWidget::updateValues() {
            //Name
            if (selectedNodes.size() > 1) {
                nameLineEdit->setText(tr("[Multiple values]"));
                nameLineEdit->setReadOnly(true);
            } else {
                QString name = selectedNodes.at(0)->getName();
                //Only set if it differs from the current value so we don't jump the cursor to the end while typing
                if (nameLineEdit->text() != name) nameLineEdit->setText(name);
                nameLineEdit->setReadOnly(false);
            }

            //UUID
            if (selectedNodes.size() > 1) {
                uuidLineEdit->setText(tr("[Multiple values]"));
            } else {
                uuidLineEdit->setText(selectedNodes.at(0)->getUuid().toString());
            }

            //Transforms
            //Average out transforms
            int numTransforms = 0;
            glm::vec3 avgPosition = glm::vec3(0.0f);
            glm::vec3 avgRotation = glm::vec3(0.0f);
            glm::vec3 avgScale = glm::vec3(0.0f);

            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getTopmostSelectedObjects()) {
                ++numTransforms;
                avgPosition += node->getTransform().getPosition();
                avgRotation += node->getTransform().getRotation();
                avgScale += node->getTransform().getScale();
            }

            avgPosition /= numTransforms;
            avgRotation /= numTransforms;
            avgScale /= numTransforms;

            prevPos = avgPosition;
            posSpinBoxes->setValue(avgPosition);
            prevRot = avgRotation;
            glm::vec3 avgRotationDeg = glm::degrees(avgRotation);
            rotSpinBoxes->setValue(avgRotationDeg);
            prevScl = avgScale;
            sclSpinBoxes->setValue(avgScale);

            //Check whether we should enable rot/scl spinboxes
            bool canMove = true;
            bool canRotate = true;
            bool canScale = true;
            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getSelectedObjects()) {
                //Blacklist bananas from rotating
                if (dynamic_cast<BananaSceneNode*>(node)) canRotate = false;

                //Whitelist bumpers/jamabars/fallout volumes for scaling
                if (!(
                            dynamic_cast<BumperSceneNode*>(node) ||
                            dynamic_cast<JamabarSceneNode*>(node) ||
                            dynamic_cast<FalloutVolumeSceneNode*>(node))) {
                    canScale = false;
                }

                //Also while we don't have a GX exporter yet, blacklist models from everything
                if (dynamic_cast<MeshSceneNode*>(node)) {
                    canMove = false;
                    canRotate = false;
                    canScale = false;
                }
            }

            posSpinBoxes->setEnabled(canMove);
            rotSpinBoxes->setEnabled(canRotate);
            sclSpinBoxes->setEnabled(canScale);
        }

        void NodeWidget::onNodeModified(SceneNode *node) {
            if (selectedNodes.contains(node)) updateValues();
        }

        void NodeWidget::onNameModified(const QString &newName) {
            for (SceneNode *node : selectedNodes) {
                node->setName(newName);
                ModelManager::modelOutliner->nodeModified(node);
            }
        }

        void NodeWidget::onPosModified(glm::vec3 newValue) {
            //Get the delta position
            glm::vec3 delta = newValue - prevPos;

            //Add the delta to each node
            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getTopmostSelectedObjects()) {
                node->setPosition(node->getPosition() + delta);
                ModelManager::modelOutliner->nodeModified(node);
            }

            prevPos = newValue;
        }

        void NodeWidget::onRotModified(glm::vec3 newValue) {
            //Get the delta rotation
            glm::vec3 delta = glm::radians(newValue) - prevRot;

            //Add the delta to each node
            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getTopmostSelectedObjects()) {
                node->setRotation(node->getRotation() + delta);
                ModelManager::modelOutliner->nodeModified(node);
            }

            prevRot = glm::radians(newValue);
        }

        void NodeWidget::onSclModified(glm::vec3 newValue) {
            //Get the delta scale
            glm::vec3 delta = newValue - prevScl;

            //Add the delta to each node
            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getTopmostSelectedObjects()) {
                node->setScale(node->getScale() + delta);
                ModelManager::modelOutliner->nodeModified(node);
            }

            prevScl = newValue;
        }
    }
}

