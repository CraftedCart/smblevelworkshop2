#include "StageEditorWindow.hpp"
#include "ui_StageEditorWindow.h"
#include <QFontDatabase>
#include <QPalette>
#include <Qt>

namespace WS2 {
    StageEditorWindow::StageEditorWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StageEditorWindow) {
        ui->setupUi(this);

        const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        statusFramerateLabel->setFont(fixedFont);
        statusFramerateLabel->setAutoFillBackground(true); //Allow changing the background color
        ui->statusBar->addWidget(statusFramerateLabel);

        connect(ui->actionQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
        connect(ui->viewportWidget, &ViewportWidget::frameRendered, this, &StageEditorWindow::viewportFrameRendered);
    }

    StageEditorWindow::~StageEditorWindow() {
        delete ui;
    }

    void StageEditorWindow::viewportFrameRendered(qint64 deltaNanoseconds) {
        float deltaMs = deltaNanoseconds / 1000000.0f;
        float fps = 1000000000.0f / deltaNanoseconds;

        statusFramerateLabel->setText(QString(tr("Delta: %1ms / Framerate: %2")).arg(
                    QString::number(deltaMs, 'f', 2),
                    QString::number(fps, 'f', 2)
                    ));

        QPalette fpsPalette = statusFramerateLabel->palette();
        if (fps < 5.0f) {
            fpsPalette.setColor(statusFramerateLabel->backgroundRole(), Qt::darkRed);
            fpsPalette.setColor(statusFramerateLabel->foregroundRole(), Qt::white);
        } else if (fps < 10.0f) {
            fpsPalette.setColor(statusFramerateLabel->backgroundRole(), Qt::red);
            fpsPalette.setColor(statusFramerateLabel->foregroundRole(), Qt::white);
        } else if (fps < 15.0f) {
            fpsPalette.setColor(statusFramerateLabel->backgroundRole(), Qt::darkYellow);
            fpsPalette.setColor(statusFramerateLabel->foregroundRole(), Qt::white);
        } else if (fps < 30.0f) {
            fpsPalette.setColor(statusFramerateLabel->backgroundRole(), Qt::yellow);
            fpsPalette.setColor(statusFramerateLabel->foregroundRole(), Qt::black);
        } else if (fps < 40.0f) {
            fpsPalette.setColor(statusFramerateLabel->backgroundRole(), Qt::darkGreen);
            fpsPalette.setColor(statusFramerateLabel->foregroundRole(), Qt::white);
        } else {
            fpsPalette.setColor(statusFramerateLabel->backgroundRole(), Qt::green);
            fpsPalette.setColor(statusFramerateLabel->foregroundRole(), Qt::black);
        }
        statusFramerateLabel->setPalette(fpsPalette);
    }
}

