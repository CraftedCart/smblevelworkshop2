/**
 * @file
 * @brief Header for the ICommandHandler interface
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_ICOMMANDHANDLER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_ICOMMANDHANDLER_HPP

#include "ws2editor_export.h"
#include "ws2editor/command/CommandReply.hpp"
#include <QString>

namespace WS2Editor {
    namespace Command {
        class WS2EDITOR_EXPORT ICommandHandler {
            public:
                virtual ~ICommandHandler() = default;

                virtual QString getCommand() = 0;

                /**
                 * @brief Executes the command
                 *
                 * @param command The command string to parse
                 *
                 * @return Whether it succeeded or not
                 */
                virtual CommandReply executeCommand(QString command) = 0;
        };
    }
}

#endif

