#include "ws2editor_export.h"

namespace WS2Editor {
    WS2EDITOR_EXPORT int ws2editorLaunch(int argc, char *argv[]);
}

int main(int argc, char *argv[]) {
    return WS2Editor::ws2editorLaunch(argc, argv);
}

