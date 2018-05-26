#include "ws2editorexampleplugin/Plugin.hpp"
#include <QAction>
#include <QMessageBox>
#include <QDebug>

namespace WS2EditorExamplePlugin {
    using namespace WS2Editor;

    bool Plugin::init() {
        qDebug() << "Hello, world! - From ws2editorexampleplugin";

        //Hook into the stage editor window by calling onStageEditorWindowConstructed after it's been created
        connect(WS2EditorInstance::getInstance(), &WS2EditorInstance::onStageEditorWindowConstructed,
                this, &Plugin::onStageEditorWindowConstructed);

        //The plugin initialized successfully, return true
        return true;
    }

    void Plugin::onStageEditorWindowConstructed(UI::StageEditorWindow *w) {
        Q_UNUSED(w); //Tell the compiler to stop yelling at me for unused parameters

        qDebug() << "ws2editorexampleplugin: Window consructed!";

        //We can whatever we want with the window now that we have a pointer to it
        //For example, show a message whenever Ctrl-P is pressed
        QAction *a = new QAction(w); //Create the action
        a->setShortcut(Qt::CTRL | Qt::Key_P); //Bind a shortcut to the action
        connect(a, &QAction::triggered, this, &Plugin::showMessage); //When the action is triggered, call showMessage

        w->addAction(a); //Add the action to the window
    }

    void Plugin::showMessage() {
        QMessageBox::information(nullptr, "ws2editorexampleplugin",
                tr("Hello, from ws2editorexampleplugin"), QMessageBox::Ok);
    }
}

