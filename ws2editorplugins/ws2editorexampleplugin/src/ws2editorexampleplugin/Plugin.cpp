#include "ws2editorexampleplugin/Plugin.hpp"
#include <QDebug>

namespace WS2EditorExamplePlugin {
    bool Plugin::init() {
        qDebug() << "Hello, world! - From ws2editorexampleplugin";

        return true;
    }
}

