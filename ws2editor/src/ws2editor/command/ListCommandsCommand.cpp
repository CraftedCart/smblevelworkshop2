#include "ws2editor/command/ListCommandsCommand.hpp"
#include "ws2editor/command/CommandInterpreter.hpp"

namespace WS2Editor {
    namespace Command {
        QString ListCommandsCommand::getCommand() {
            return "list-commands";
        }

        CommandReply ListCommandsCommand::executeCommand(QString command) {
            Q_UNUSED(command);

            QString commandList = "";

            for (ICommandHandler *handler : CommandInterpreter::getInstance()->getRegisteredCommands()) {
                commandList += handler->getCommand() + "\n";
            }

            //Remove trailing newline
            commandList.chop(1);

            return CommandReply(true, commandList);
        }
    }
}

