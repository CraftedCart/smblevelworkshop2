/**
 * @file
 * @brief Header for the SettingsDialog class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_UI_SETTINGSDIALOG_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_UI_SETTINGSDIALOG_HPP

#include "ws2editor_export.h"
#include <QDialog>

namespace Ui {
    class WS2EDITOR_EXPORT SettingsDialog;
}

namespace WS2Editor {
    namespace UI {
        class WS2EDITOR_EXPORT SettingsDialog : public QDialog {
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

