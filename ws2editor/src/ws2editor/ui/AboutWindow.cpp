#include "ws2editor/ui/AboutWindow.hpp"
#include "ws2editor/ui_AboutWindow.h"
#include <QDesktopServices>

#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

namespace WS2Editor {
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

