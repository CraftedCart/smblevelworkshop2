/**
 * @file
 * @brief Header for the AboutWindow class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_ABOUTWINDOW_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_ABOUTWINDOW_HPP

#include "ws2editor_export.h"
#include <QDialog>

namespace Ui {
    class WS2EDITOR_EXPORT AboutWindow;
}

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT AboutWindow : public QDialog {
            Q_OBJECT

            public:
                explicit AboutWindow(QWidget *parent = 0);
                ~AboutWindow();

            private:
                Ui::AboutWindow *ui;
        };
    }
}

#endif

