/**
 * @file
 * @brief Header for the CommandReply class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDREPLY_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_COMMAND_COMMANDREPLY_HPP

#include <QString>

namespace WS2Editor {
    namespace Command {
        class CommandReply {
            protected:
                bool success;
                QString translatedErrorString;

            public:
                CommandReply(bool success, QString translatedErrorString = "");

                bool getSuccess();
                QString getTranslatedErrorString();
        };
    }
}

#endif

