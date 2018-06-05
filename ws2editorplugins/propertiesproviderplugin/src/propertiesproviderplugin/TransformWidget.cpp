#include "propertiesproviderplugin/TransformWidget.hpp"
#include "ws2editor/widget/Vec3DraggableSpinBoxes.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include <QLabel>

namespace WS2EditorPlugins {
    namespace PropertiesProviderPlugin {
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::UI;
        using namespace WS2Common::Scene;

        TransformWidget::TransformWidget(QVector<SceneNode*> &nodes, QString label, QWidget *parent) :
            CollapsableWidget(label, 100, parent) {
            QVBoxLayout *sectionLayout = new QVBoxLayout();
            sectionLayout->setMargin(0);
            setContentLayout(*sectionLayout);

            //Create widgets
            sectionLayout->addWidget(new QLabel(tr("Position")));
            Vec3DraggableSpinBoxes *posSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(posSpinBoxes);

            sectionLayout->addWidget(new QLabel(tr("Rotation")));
            Vec3DraggableSpinBoxes *rotSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(rotSpinBoxes);

            sectionLayout->addWidget(new QLabel(tr("Scale")));
            Vec3DraggableSpinBoxes *sclSpinBoxes = new Vec3DraggableSpinBoxes();
            sectionLayout->addWidget(sclSpinBoxes);

            auto updateSceneNodeWidgets = [nodes, posSpinBoxes, rotSpinBoxes, sclSpinBoxes]() {
                //Average out transforms
                int numTransforms = 0;
                glm::vec3 avgPosition = glm::vec3(0.0f);
                glm::vec3 avgRotation = glm::vec3(0.0f);
                glm::vec3 avgScale = glm::vec3(0.0f);

                for (SceneNode *node : nodes) {
                    ++numTransforms;
                    avgPosition += node->getTransform().getPosition();
                    avgRotation += node->getTransform().getRotation();
                    avgScale += node->getTransform().getScale();
                }

                avgPosition /= numTransforms;
                avgRotation /= numTransforms;
                avgScale /= numTransforms;

                posSpinBoxes->setValue(avgPosition);
                rotSpinBoxes->setValue(avgRotation);
                sclSpinBoxes->setValue(avgScale);
            };

            updateSceneNodeWidgets();

            //Set the object reference to posSpinBoxes so the connection is destroyed when the posSpinBoxes is deleted
            //(Which happens when the selection is changed, as the spin boxes are recreated)
            connect(ModelManager::modelOutliner, &ModelOutliner::onNodeModified, posSpinBoxes, updateSceneNodeWidgets);
        }
    }
}

