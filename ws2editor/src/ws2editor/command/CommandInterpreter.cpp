#include "ws2editor/command/CommandInterpreter.hpp"
#include "ws2editor/command/ListCommandsCommand.hpp"
#include "ws2editor/command/QuitCommand.hpp"
#include "ws2editor/command/RestartCommand.hpp"
#include <QStringList>
#include <QCoreApplication>
#include <QDebug>

namespace WS2Editor {
    namespace Command {
        //Define statics
        CommandInterpreter *CommandInterpreter::instance;

        void CommandInterpreter::createInstance() {
            instance = new CommandInterpreter;
        }

        void CommandInterpreter::destroyInstance() {
            delete instance;
        }

        CommandInterpreter* CommandInterpreter::getInstance() {
            return instance;
        }

        CommandInterpreter::CommandInterpreter() {
            //Register built-in commands

            registerCommand(new ListCommandsCommand);
            registerCommand(new QuitCommand);
            registerCommand(new RestartCommand);
        }

        CommandInterpreter::~CommandInterpreter() {
            //Unregister all commands
            qDeleteAll(commands);
        }

        void CommandInterpreter::registerCommand(ICommandHandler *commandHandler) {
            commands.append(commandHandler);
        }

        QVector<ICommandHandler*>& CommandInterpreter::getRegisteredCommands() {
            return commands;
        }

        CommandReply CommandInterpreter::runCommand(QString command) {
            qDebug().noquote() << QString("Command: %1").arg(command);

            QString first = command.split(" ").at(0); //Get the first word

            //Check the first word against registered command handlers
            for (ICommandHandler *handler : commands) {
                if (first == handler->getCommand()) {
                    //Found the command
                    return handler->executeCommand(command);
                }
            }

            //Couldn't find any matching commands
            return CommandReply(false, QCoreApplication::translate("CommandInterpreter", "Command not found: %1").arg(first));
        }
    }
}

