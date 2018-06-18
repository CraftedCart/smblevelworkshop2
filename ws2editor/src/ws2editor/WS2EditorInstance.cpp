#include "ws2editor/WS2EditorInstance.hpp"
#include "ws2common/resource/ResourceMesh.hpp"

namespace WS2Editor {
    //Define statics
    WS2EditorInstance *WS2EditorInstance::instance = nullptr;

    void WS2EditorInstance::createInstance(int &argc, char *argv[]) {
        instance = new WS2EditorInstance(argc, argv);

        using namespace WS2Common::Scene;
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
        //Remove and unregister all export provider
        //Deleting export providers will automatically unregister them
        qDeleteAll(exportProviders);

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

    void WS2EditorInstance::registerExportProvider(IExportProvider *provider) {
        exportProviders.append(provider);

        //Remove the provider from the vector when it's deleted
        connect(provider, &QObject::destroyed, [this](QObject *obj) {
                exportProviders.removeAll(static_cast<IExportProvider*>(obj));
                });
    }

    QVector<IExportProvider*>& WS2EditorInstance::getExportProviders() {
        return exportProviders;
    }

    const QVector<IExportProvider*>& WS2EditorInstance::getExportProviders() const {
        return exportProviders;
    }

    int WS2EditorInstance::execApp() {
        int ret = app->exec();

        return ret;
    }
}

