#include "ws2editor/widget/DraggableDoubleSpinBox.hpp"
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>

namespace WS2Editor {
    namespace Widget {
        DraggableDoubleSpinBox::DraggableDoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent) {
            upDownPixmap = QPixmap(":/ws2editor/icons/ArrowsUpDown.png");

            setCursor(Qt::SizeHorCursor);
        }

        void DraggableDoubleSpinBox::mousePressEvent(QMouseEvent *event) {
            preGrabMousePos = event->globalPos();

            QCursor::setPos(parentWidget()->mapToGlobal(pos()));
            setCursor(Qt::BlankCursor);
        }

        void DraggableDoubleSpinBox::mouseReleaseEvent(QMouseEvent *event) {
            Q_UNUSED(event);

            setCursor(Qt::SizeHorCursor);
            QCursor::setPos(preGrabMousePos);
        }

        void DraggableDoubleSpinBox::mouseMoveEvent(QMouseEvent *event) {
            double multiplier;

            if ((event->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) {
                multiplier = 0.5;
            } else if ((event->modifiers() & Qt::AltModifier) == Qt::AltModifier) {
                multiplier = 0.01;
            } else {
                multiplier = 0.1;
            }

            setValue(value() + multiplier * event->pos().x());
            QCursor::setPos(parentWidget()->mapToGlobal(pos()));
        }

        void DraggableDoubleSpinBox::paintEvent(QPaintEvent *event) {
            Q_UNUSED(event);

            QPainter painter(this);

            QStyleOptionSpinBox opt;
            opt.initFrom(this);

            //Draw the background
            painter.fillRect(0, 0, width(), height(), opt.palette.base());

            QRect subControlRect = style()->subControlRect(QStyle::CC_SpinBox, &opt, QStyle::SC_SpinBoxUp, this);

            //Draw a line between the line input and arrow section
            painter.fillRect(subControlRect.x() - 2, 0, 2, height(), opt.palette.mid());

            //Draw the up/down arrow image
            painter.drawPixmap(
                    subControlRect.x() + 2,
                    height() / 2 - subControlRect.height() / 2 - 1,
                    subControlRect.width() - 4,
                    subControlRect.width() - 4,
                    upDownPixmap
                    );
        }
    }
}

