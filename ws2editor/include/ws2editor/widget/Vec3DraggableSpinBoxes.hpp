/**
 * @file
 * @brief Header for the Vec3DraggableDoubleSpinBox class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_VEC3DRAGGABLESPINBOXES_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_VEC3DRAGGABLESPINBOXES_HPP

#include "ws2editor_export.h"
#include "ws2editor/widget/DraggableDoubleSpinBox.hpp"
#include <glm/glm.hpp>

namespace WS2Editor {
    namespace Widget {
        class WS2EDITOR_EXPORT Vec3DraggableSpinBoxes : public QWidget {
            Q_OBJECT

            protected:
                DraggableDoubleSpinBox *xSpinBox;
                DraggableDoubleSpinBox *ySpinBox;
                DraggableDoubleSpinBox *zSpinBox;

            protected:
                void onValueChanged();

            public:
                explicit Vec3DraggableSpinBoxes(QWidget *parent = nullptr);

                void setValue(glm::vec3 &value);
                glm::vec3 getValue();

            signals:
                void valueChanged(glm::vec3 value);
        };
    }
}

#endif

