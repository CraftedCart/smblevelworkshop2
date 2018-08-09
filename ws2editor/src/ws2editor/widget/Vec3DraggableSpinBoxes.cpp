#include "ws2editor/widget/Vec3DraggableSpinBoxes.hpp"
#include "ws2common/MathUtils.hpp"
#include <QHBoxLayout>
#include <cfloat>

namespace WS2Editor {
    namespace Widget {
        Vec3DraggableSpinBoxes::Vec3DraggableSpinBoxes(QWidget *parent) : QWidget(parent) {
            QHBoxLayout *layout = new QHBoxLayout();
            layout->setMargin(0);
            layout->setSpacing(0);
            setLayout(layout);

            QWidget *rWidget = new QWidget();
            rWidget->setObjectName("xColorMarkerWidget");
            layout->addWidget(rWidget);

            xSpinBox = new DraggableDoubleSpinBox();
            xSpinBox->setRange(-FLT_MAX, FLT_MAX);
            xSpinBox->setMinimumWidth(12);
            layout->addWidget(xSpinBox);

            layout->addSpacing(4);

            QWidget *gWidget = new QWidget();
            gWidget->setObjectName("yColorMarkerWidget");
            layout->addWidget(gWidget);

            ySpinBox = new DraggableDoubleSpinBox();
            ySpinBox->setRange(-FLT_MAX, FLT_MAX);
            ySpinBox->setMinimumWidth(12);
            layout->addWidget(ySpinBox);

            layout->addSpacing(4);

            QWidget *bWidget = new QWidget();
            bWidget->setObjectName("zColorMarkerWidget");
            layout->addWidget(bWidget);

            zSpinBox = new DraggableDoubleSpinBox();
            zSpinBox->setRange(-FLT_MAX, FLT_MAX);
            zSpinBox->setMinimumWidth(12);
            layout->addWidget(zSpinBox);

            connect(xSpinBox, QOverload<double>::of(&DraggableDoubleSpinBox::valueChanged),
                    this, &Vec3DraggableSpinBoxes::onValueChanged);
            connect(ySpinBox, QOverload<double>::of(&DraggableDoubleSpinBox::valueChanged),
                    this, &Vec3DraggableSpinBoxes::onValueChanged);
            connect(zSpinBox, QOverload<double>::of(&DraggableDoubleSpinBox::valueChanged),
                    this, &Vec3DraggableSpinBoxes::onValueChanged);
        }

        void Vec3DraggableSpinBoxes::setValue(glm::vec3 &value) {
            using namespace WS2Common;

            //Don't set the value if it's already pretty much equal - the user could be typing in the spin box and
            //setting it would reset the caret
            if (!MathUtils::areAlmostEqual(value.x, xSpinBox->value())) xSpinBox->setValue(value.x);
            if (!MathUtils::areAlmostEqual(value.y, ySpinBox->value())) ySpinBox->setValue(value.y);
            if (!MathUtils::areAlmostEqual(value.z, zSpinBox->value())) zSpinBox->setValue(value.z);
        }

        glm::vec3 Vec3DraggableSpinBoxes::getValue() {
            return glm::vec3(
                    xSpinBox->value(),
                    ySpinBox->value(),
                    zSpinBox->value()
                    );
        }

        void Vec3DraggableSpinBoxes::setPrefix(const QString &prefix) {
            xSpinBox->setPrefix(prefix);
            ySpinBox->setPrefix(prefix);
            zSpinBox->setPrefix(prefix);
        }

        void Vec3DraggableSpinBoxes::setSuffix(const QString &suffix) {
            xSpinBox->setSuffix(suffix);
            ySpinBox->setSuffix(suffix);
            zSpinBox->setSuffix(suffix);
        }

        void Vec3DraggableSpinBoxes::onValueChanged() {
            emit valueChanged(glm::vec3(
                        xSpinBox->value(),
                        ySpinBox->value(),
                        zSpinBox->value()
                        ));
        }
    }
}

