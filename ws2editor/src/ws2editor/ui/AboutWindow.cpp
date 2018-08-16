#include "ws2editor/ui/AboutWindow.hpp"
#include "ws2editor/ui_AboutWindow.h"
#include <QDesktopServices>

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

namespace WS2Editor {
    namespace UI {
        AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
            ui->setupUi(this);
            ui->compileDateLabel->setText(tr("Compiled on %1").arg(TO_STRING(SMBLEVELWORKSHOP2_COMPILE_TIMESTAMP)));
            ui->versionLabel->setText(tr("v%1").arg(TO_STRING(SMBLEVELWORKSHOP2_VERSION_STRING)));

            connect(ui->gitlabButton, &QPushButton::clicked, [](){ QDesktopServices::openUrl(QUrl("https://gitlab.com/CraftedCart/smblevelworkshop2")); });
        }

        AboutWindow::~AboutWindow() {
            delete ui;
        }
    }
}

