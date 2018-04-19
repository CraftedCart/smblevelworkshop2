#include "ws2editor/command/RestartCommand.hpp"
#include <QApplication>
#include <QProcess>

namespace WS2Editor {
    namespace Command {
        QString RestartCommand::getCommand() {
            return "restart";
        }

        bool RestartCommand::executeCommand(QString command) {
            Q_UNUSED(command);

            QApplication::exit();
            QProcess::startDetached(QApplication::arguments()[0], QApplication::arguments());
            return true;
        }
    }
}

