/**
 * @file
 * @brief Header for the AboutWindow class
 */

#ifndef SMBLEVELWORKSHOP2_UI_ABOUTWINDOW_HPP
#define SMBLEVELWORKSHOP2_UI_ABOUTWINDOW_HPP

#include <QDialog>

namespace Ui {
    class AboutWindow;
}

namespace WS2 {
    namespace UI {
        class AboutWindow : public QDialog {
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

