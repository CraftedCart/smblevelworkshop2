#include "propertiesproviderplugin/TransformWidget.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include <QLabel>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Editor::Project;
        using namespace WS2Common::Scene;

        TransformWidget::TransformWidget(QVector<SceneNode*> &nodes, QString label, QWidget *parent) :
            CollapsableWidget(label, 100, parent)
        {
            selectedNodes = nodes;

            QVBoxLayout *sectionLayout = new QVBoxLayout();
            sectionLayout->setMargin(0);
            setContentLayout(*sectionLayout);

            //Create widgets
            sectionLayout->addWidget(new QLabel(tr("Position")));
            posSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(posSpinBoxes);

            sectionLayout->addWidget(new QLabel(tr("Rotation")));
            rotSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(rotSpinBoxes);

            sectionLayout->addWidget(new QLabel(tr("Scale")));
            sclSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(sclSpinBoxes);

            updateValues();

            //Set the object reference to posSpinBoxes so the connection is destroyed when the posSpinBoxes is deleted
            //(Which happens when the selection is changed, as the spin boxes are recreated)
            connect(ModelManager::modelOutliner, &ModelOutliner::onNodeModified, this, &TransformWidget::onNodeModified);

            connect(posSpinBoxes, &Vec3DraggableSpinBoxes::valueChanged, this, &TransformWidget::onPosModified);
            connect(rotSpinBoxes, &Vec3DraggableSpinBoxes::valueChanged, this, &TransformWidget::onRotModified);
            connect(sclSpinBoxes, &Vec3DraggableSpinBoxes::valueChanged, this, &TransformWidget::onSclModified);
        }

        void TransformWidget::updateValues() {
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
            rotSpinBoxes->setValue(avgRotation);
            prevScl = avgScale;
            sclSpinBoxes->setValue(avgScale);
        }

        void TransformWidget::onNodeModified(SceneNode *node) {
            if (selectedNodes.contains(node)) updateValues();
        }

        void TransformWidget::onPosModified(glm::vec3 newValue) {
            //Get the delta position
            glm::vec3 delta = newValue - prevPos;

            //Add the delta to each node
            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getTopmostSelectedObjects()) {
                node->setPosition(node->getPosition() + delta);
                ModelManager::modelOutliner->nodeModified(node);
            }

            prevPos = newValue;
        }

        void TransformWidget::onRotModified(glm::vec3 newValue) {
            //Get the delta rotation
            glm::vec3 delta = newValue - prevRot;

            //Add the delta to each node
            for (SceneNode *node : ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getTopmostSelectedObjects()) {
                node->setRotation(node->getRotation() + delta);
                ModelManager::modelOutliner->nodeModified(node);
            }

            prevRot = newValue;
        }

        void TransformWidget::onSclModified(glm::vec3 newValue) {
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

