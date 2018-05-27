/**
 * @file
 * @brief Header for the CommandInterpreter class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDINTERPRETER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDINTERPRETER_HPP

#include "ws2editor_export.h"
#include "ws2editor/command/ICommandHandler.hpp"
#include "ws2editor/command/CommandReply.hpp"
#include <QString>
#include <QVector>

namespace WS2Editor {
    namespace Command {
        class WS2EDITOR_EXPORT CommandInterpreter {
            protected:
                WS2EDITOR_EXPORT static CommandInterpreter *instance;

                QVector<ICommandHandler*> commands;

                CommandInterpreter();
                ~CommandInterpreter();

            public:
                WS2EDITOR_EXPORT static void createInstance();
                WS2EDITOR_EXPORT static void destroyInstance();
                WS2EDITOR_EXPORT static CommandInterpreter* getInstance();

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

