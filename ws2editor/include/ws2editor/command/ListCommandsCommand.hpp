/**
 * @file
 * @brief Header for the ListCommandsCommand class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_LISTCOMMANDSCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_LISTCOMMANDSCOMMAND_HPP

#include "ws2editor/command/ICommandHandler.hpp"

namespace WS2Editor {
    namespace Command {
        class ListCommandsCommand : public ICommandHandler {
            public:
                virtual QString getCommand() override;
                virtual CommandReply executeCommand(QString command) override;
        };
    }
}

#endif

