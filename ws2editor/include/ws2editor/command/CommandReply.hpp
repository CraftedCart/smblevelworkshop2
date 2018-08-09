/**
 * @file
 * @brief Header for the CommandReply class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDREPLY_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDREPLY_HPP

#include "ws2editor_export.h"
#include <QString>

namespace WS2Editor {
    namespace Command {
        class WS2EDITOR_EXPORT CommandReply {
            protected:
                bool success;
                QString translatedMessage;

            public:
                CommandReply(bool success, QString translatedMessage = "");

                bool getSuccess();
                QString getTranslatedMessage();
        };
    }
}

#endif

