/**
 * @file
 * @brief Header for the CommandInterpreter class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDINTERPRETER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDINTERPRETER_HPP

#include "ws2editor/command/ICommandHandler.hpp"
#include "ws2editor/command/CommandReply.hpp"
#include <QString>
#include <QVector>

namespace WS2Editor {
    namespace Command {
        class CommandInterpreter {
            protected:
                static CommandInterpreter *instance;

                QVector<ICommandHandler*> commands;

                CommandInterpreter();
                ~CommandInterpreter();

            public:
                static void createInstance();
                static void destroyInstance();
                static CommandInterpreter* getInstance();

                void registerCommand(ICommandHandler *commandHandler);
                QVector<ICommandHandler*>& getRegisteredCommands();

                /**
                 * @brief Executes the command form the given string
                 *
                 * @param command The command to parse and execute
                 *
                 * @return Whether it succeeded or not
                 */
                CommandReply runCommand(QString command);
        };
    }
}

#endif

