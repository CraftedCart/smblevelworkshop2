#include "ws2editor/ui/SettingsDialog.hpp"
#include "ws2editor/ui_SettingsDialog.h"
#include "ws2editor/Config.hpp"

namespace WS2Editor {
    namespace UI {
        SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog) {
            ui->setupUi(this);

            //Pull in values from Config and set the values in the UI
            //Also make it do when values are changed, update the config
            ui->cameraSpeedSpinBox->setValue(Config::cameraPosSpeed);
            connect(ui->cameraSpeedSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraPosSpeed = value;});

            ui->cameraSpeedUpMultiplierSpinBox->setValue(Config::cameraPosSpeedUpMultiplier);
            connect(ui->cameraSpeedUpMultiplierSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraPosSpeedUpMultiplier = value;});

            ui->cameraSlowDownMultiplierSpinBox->setValue(Config::cameraPosSlowDownMultiplier);
            connect(ui->cameraSlowDownMultiplierSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraPosSlowDownMultiplier = value;});

            ui->cameraRotationSpeedSpinBox->setValue(Config::cameraRotSpeed);
            connect(ui->cameraRotationSpeedSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraRotSpeed = value;});

            ui->cameraInertiaSpinBox->setValue(Config::cameraInertia);
            connect(ui->cameraInertiaSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraInertia = value;});

            ui->cameraFovSpinBox->setValue(Config::cameraFov);
            connect(ui->cameraFovSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraFov = value;});

            ui->cameraNearSpinBox->setValue(Config::cameraNear);
            connect(ui->cameraNearSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraNear = value;});

            ui->cameraFarSpinBox->setValue(Config::cameraFar);
            connect(ui->cameraFarSpinBox, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                    [](double value) {Config::cameraFar = value;});
        }

        SettingsDialog::~SettingsDialog() {
            delete ui;
        }
    }
}


