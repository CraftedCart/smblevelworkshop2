#include "ws2editor/command/TranslateSelectedCommand.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include <QCoreApplication>

namespace WS2Editor {
    namespace Command {
        using namespace WS2Common::Scene;

        QString TranslateSelectedCommand::getCommand() {
            return "translate-selected";
        }

        CommandReply TranslateSelectedCommand::executeCommand(QString command) {
            Q_UNUSED(command);

            using namespace WS2Editor::Project;

            QStringList args = command.split(" ");
            if (args.size() != 4) {
                //Expected command + 3 args (x/y/z)
                return CommandReply(false,
                        QCoreApplication::translate("TranslateSelectedCommand", "Expected 3 args: delta-x delta-y delta-z")
                        );
            }

            //Parse args
            glm::vec3 delta;
            bool success;

            delta.x = args.at(1).toFloat(&success);
            if (!success) return CommandReply(false,
                    QCoreApplication::translate("TranslateSelectedCommand", "Failed to parse delta-x")
                    );

            delta.y = args.at(2).toFloat(&success);
            if (!success) return CommandReply(false,
                    QCoreApplication::translate("TranslateSelectedCommand", "Failed to parse delta-y")
                    );

            delta.z = args.at(3).toFloat(&success);
            if (!success) return CommandReply(false,
                    QCoreApplication::translate("TranslateSelectedCommand", "Failed to parse delta-z")
                    );

            QVector<SceneNode*> &selected =
                ProjectManager::getActiveProject()->getScene()->getSelectionManager()->getSelectedObjects();

            for (SceneNode *node : selected) {
                node->getTransform().translate(delta);
                UI::ModelManager::modelOutliner->nodeModified(node);
            }

            return CommandReply(true);
        }
    }
}

