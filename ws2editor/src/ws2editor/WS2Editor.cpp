#include "ws2editor/WS2Editor.hpp"
#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2Editor {
    //Define extern members
    Task::TaskManager *ws2TaskManager;
    QApplication *ws2App;
    bool qAppRunning = false;

    void ws2Init(int &argc, char *argv[]) {
        using namespace WS2Common::Resource;

        qRegisterMetaType<QVector<ResourceMesh*>>("QVector<ResourceMesh*>");

        ws2App = new QApplication(argc, argv);
        ws2TaskManager = new Task::TaskManager();
    }

    void ws2Destroy() {
        delete ws2TaskManager;
        delete ws2App;

        //Application termination will unload the plugins
    }

    QVector<QPluginLoader*>& ws2GetLoadedPlugins() {
        static QVector<QPluginLoader*> loadedPlugins;
        return loadedPlugins;
    }

    QVector<QPluginLoader*>& ws2GetInitializedPlugins() {
        static QVector<QPluginLoader*> initializedPlugins;
        return initializedPlugins;
    }

    QVector<QPluginLoader*>& ws2GetFailedPlugins() {
        static QVector<QPluginLoader*> failedPlugins;
        return failedPlugins;
    }
}

