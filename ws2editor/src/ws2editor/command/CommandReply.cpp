#include "ws2editor/command/CommandReply.hpp"

namespace WS2Editor {
    namespace Command {
        CommandReply::CommandReply(bool success, QString translatedErrorString) :
            success(success),
            translatedErrorString(translatedErrorString) {}

        bool CommandReply::getSuccess() {
            return success;
        }

        QString CommandReply::getTranslatedErrorString() {
            return translatedErrorString;
        }
    }
}

