#include "ws2editor/command/QuitCommand.hpp"
#include <QApplication>

namespace WS2Editor {
    namespace Command {
        QString QuitCommand::getCommand() {
            return "quit";
        }

        CommandReply QuitCommand::executeCommand(QString command) {
            Q_UNUSED(command);

            QApplication::exit();
            return CommandReply(true);
        }
    }
}

