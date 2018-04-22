#include "ws2editor/command/ToggleDebugDrawingCommand.hpp"
#include "ws2editor/Config.hpp"
#include <QApplication>

namespace WS2Editor {
    namespace Command {
        QString ToggleDebugDrawingCommand::getCommand() {
            return "toggle-debug-drawing";
        }

        CommandReply ToggleDebugDrawingCommand::executeCommand(QString command) {
            Q_UNUSED(command);

            Config::enablePhysicsDebugDrawing = !Config::enablePhysicsDebugDrawing;
            return CommandReply(true);
        }
    }
}

