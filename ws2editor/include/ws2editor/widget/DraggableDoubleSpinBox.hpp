/**
 * @file
 * @brief Header for the DraggableDoubleSpinBox class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_DRAGGABLEDOUBLESPINBOX_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WIDGET_DRAGGABLEDOUBLESPINBOX_HPP

#include "ws2editor_export.h"
#include <QDoubleSpinBox>

namespace WS2Editor {
    namespace Widget {
        class WS2EDITOR_EXPORT DraggableDoubleSpinBox : public QDoubleSpinBox {
            Q_OBJECT

            protected:
                QPoint preGrabMousePos;
                QPixmap upDownPixmap;

            protected:
                virtual void mousePressEvent(QMouseEvent *event) override;
                virtual void mouseReleaseEvent(QMouseEvent *event) override;
                virtual void mouseMoveEvent(QMouseEvent *event) override;
                virtual void paintEvent(QPaintEvent *event) override;

            public:
                explicit DraggableDoubleSpinBox(QWidget *parent = nullptr);
        };
    }
}

#endif

