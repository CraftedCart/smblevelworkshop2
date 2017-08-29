/**
 * @file
 * @brief Header for the SettingsDialog class
 */

#ifndef SMBLEVELWORKSHOP2_UI_SETTINGSDIALOG_HPP
#define SMBLEVELWORKSHOP2_UI_SETTINGSDIALOG_HPP

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

namespace WS2 {
    namespace UI {
        class SettingsDialog : public QDialog {
            Q_OBJECT

            public:
                explicit SettingsDialog(QWidget *parent = 0);
                ~SettingsDialog();

            private:
                Ui::SettingsDialog *ui;
        };
    }
}

#endif

