#include "ws2editor/ui/StatusPopupWidget.hpp"
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>

namespace WS2Editor {
    namespace UI {
        StatusPopupWidget::StatusPopupWidget(QPoint pos, QString text, QString labelName, QWidget *parent) : QWidget(parent) {
            setAttribute(Qt::WA_DeleteOnClose);
            setWindowFlags(windowFlags() | Qt::Popup);

            QHBoxLayout *layout = new QHBoxLayout();
            layout->setContentsMargins(0, 0, 0, 0);
            setLayout(layout);

            label = new QLabel(this);
            label->setText(text);
            label->setObjectName(labelName);
            layout->addWidget(label);

            //Make sure the widget is on screen
            adjustSize();

            QRect desktop = QApplication::desktop()->screenGeometry();
            if (pos.x() + width() > desktop.width()) pos.setX(desktop.width() - width());
            if (pos.y() + height() > desktop.height()) pos.setY(desktop.height() - height());

            move(pos);
        }
    }
}

