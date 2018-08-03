/**
 * @file
 * @brief Header for the TranslateSelectedCommand class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_TRANSLATESELECTEDCOMMAND_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_TRANSLATESELECTEDCOMMAND_HPP

#include "ws2editor_export.h"
#include "ws2editor/command/ICommandHandler.hpp"

namespace WS2Editor {
    namespace Command {
        class WS2EDITOR_EXPORT TranslateSelectedCommand : public ICommandHandler {
            public:
                virtual QString getCommand() override;
                virtual CommandReply executeCommand(QString command) override;
        };
    }
}

#endif

