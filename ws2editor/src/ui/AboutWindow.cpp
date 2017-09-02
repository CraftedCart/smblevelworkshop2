#include "ui/AboutWindow.hpp"
#include "ui_AboutWindow.h"
#include <QDesktopServices>

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

namespace WS2 {
    namespace UI {
        AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
            ui->setupUi(this);
            ui->compileDateLabel->setText(tr("Compiled on " TO_STRING(COMPILE_TIMESTAMP)));

            connect(ui->githubButton, &QPushButton::clicked, [](){ QDesktopServices::openUrl(QUrl("https://github.com/CraftedCart/smblevelworkshop2")); });
        }

        AboutWindow::~AboutWindow() {

        }
    }
}

