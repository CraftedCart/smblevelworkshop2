#include "ws2editor/command/CommandReply.hpp"

namespace WS2Editor {
    namespace Command {
        CommandReply::CommandReply(bool success, QString translatedMessage) :
            success(success),
            translatedMessage(translatedMessage) {}

        bool CommandReply::getSuccess() {
            return success;
        }

        QString CommandReply::getTranslatedMessage() {
            return translatedMessage;
        }
    }
}

