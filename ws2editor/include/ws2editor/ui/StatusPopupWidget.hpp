/**
 * @file
 * @brief Header for the StatusPopupWidget class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_STATUSPOPUPWIDGET_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_STATUSPOPUPWIDGET_HPP

#include "ws2editor_export.h"
#include <QLabel>

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT StatusPopupWidget : public QWidget {
            Q_OBJECT

            protected:
                QLabel *label;

            public:
                StatusPopupWidget(QPoint pos, QString text, QString labelName, QWidget *parent);
        };
    }
}

#endif

