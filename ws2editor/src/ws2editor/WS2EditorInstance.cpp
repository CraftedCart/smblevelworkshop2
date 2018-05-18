#include "ws2editor/WS2EditorInstance.hpp"
#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2Editor {
    //Define statics
    WS2EditorInstance *WS2EditorInstance::instance = nullptr;

    void WS2EditorInstance::createInstance(int &argc, char *argv[]) {
        instance = new WS2EditorInstance(argc, argv);
    }

    WS2EditorInstance* WS2EditorInstance::getInstance() {
        return instance;
    }

    void WS2EditorInstance::destroyInstance() {
        delete instance;
        instance = nullptr;
    }

    WS2EditorInstance::WS2EditorInstance(int &argc, char *argv[]) {
        using namespace WS2Common::Resource;

        qRegisterMetaType<QVector<ResourceMesh*>>("QVector<ResourceMesh*>");

        app = new QApplication(argc, argv);
        taskManager = new Task::TaskManager();
    }

    WS2EditorInstance::~WS2EditorInstance() {
        delete taskManager;
        delete app;

        //Application termination will unload the plugins
    }

    QApplication* WS2EditorInstance::getApp() {
        return app;
    }

    Task::TaskManager* WS2EditorInstance::getTaskManager() {
        return taskManager;
    }

    QVector<QPluginLoader*>& WS2EditorInstance::getLoadedPlugins() {
        static QVector<QPluginLoader*> loadedPlugins;
        return loadedPlugins;
    }

    QVector<QPluginLoader*>& WS2EditorInstance::getInitializedPlugins() {
        static QVector<QPluginLoader*> initializedPlugins;
        return initializedPlugins;
    }

    QVector<QPluginLoader*>& WS2EditorInstance::getFailedPlugins() {
        static QVector<QPluginLoader*> failedPlugins;
        return failedPlugins;
    }

    int WS2EditorInstance::execApp() {
        int ret = app->exec();

        return ret;
    }
}

