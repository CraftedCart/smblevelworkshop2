#include "ws2editor/WS2Editor.hpp"
#include "ws2editor/ui/StageEditorWindow.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/command/CommandInterpreter.hpp"
#include "ws2editor/plugin/IEditorPlugin.hpp"
#include "ws2common/MessageHandler.hpp"
#include <QFile>
#include <QSurfaceFormat>
#include <QSplashScreen>
#include <QTranslator>

int main(int argc, char *argv[]) {
    qInstallMessageHandler(WS2Common::messageHandler);

    //Init WS2
    WS2Editor::ws2Init(argc, argv);

    //Load translations
    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("lang"), QLatin1String("_"), QDir(QCoreApplication::applicationDirPath()).filePath("../share/ws2editor/lang"))) {
        WS2Editor::ws2App->installTranslator(&translator);
    } else if (translator.load(QLocale(), QLatin1String("lang"), QLatin1String("_"), QCoreApplication::applicationDirPath())) {
        //If the software was never installed after build, the translations will be alongside the executable
        WS2Editor::ws2App->installTranslator(&translator);
    } else if (translator.load("lang_en_US", QDir(QCoreApplication::applicationDirPath()).filePath("../share/ws2editor/lang"))) {
        //If we can't find a suitable translation, try en_US
        WS2Editor::ws2App->installTranslator(&translator);
    } else if (translator.load("lang_en_US", QCoreApplication::applicationDirPath())) {
        //If we can't find a suitable translation, try en_US - if not intalled
        WS2Editor::ws2App->installTranslator(&translator);
    }

    //Init command interpreter
    WS2Editor::Command::CommandInterpreter::createInstance();

    //Splash screen
    QPixmap pixmap(":/Workshop2/Images/banner.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(QApplication::translate("main", "Initializing WS2"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    splash.showMessage(QApplication::translate("main", "Setting default OpenGL format"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    //Set default format
    QSurfaceFormat fmt;

    //Use core profile for OpenGL 3.3
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);

    fmt.setSamples(4); //4x MSAA
    fmt.setSwapInterval(0); //VSync wrecks havok with the framerate in Qt

    QSurfaceFormat::setDefaultFormat(fmt);

    //Splash message
    splash.showMessage(QApplication::translate("main", "Setting style"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    QFile styleFile(":/Styles/FlatDark/FlatDark.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    styleFile.close();
    WS2Editor::ws2App->setStyleSheet(style);

    //Load plugins
    splash.showMessage(QApplication::translate("main", "Finding plugins"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    qDebug() << "Loading plugins";
    QDir pluginsDir = QDir(QApplication::applicationDirPath());
    pluginsDir.cdUp();
    pluginsDir.cd("share");
    pluginsDir.cd("ws2editor");
    //Try and make the plugins dir if it doesn't exist
    pluginsDir.mkdir("plugins");
    pluginsDir.cd("plugins");

    for (QString fileName : pluginsDir.entryList(QDir::Files)) {
        qDebug().noquote() << "Loading plugin" << fileName;
        splash.showMessage(QApplication::translate("main", "Loading plugin %1").arg(fileName), Qt::AlignRight | Qt::AlignBottom, Qt::white);
        WS2Editor::ws2App->processEvents();

        QPluginLoader *loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader->instance();

        if (plugin) {
            WS2Editor::ws2GetLoadedPlugins() << loader;
        } else {
            qDebug().noquote() << "Failed to load plugin" << fileName << "-" << loader->errorString();
            loader->unload();

            WS2Editor::ws2GetFailedPlugins() << loader;
        }
    }

    //Now that we've loaded all the plugins, try to initialize them
    for (QPluginLoader *loader : WS2Editor::ws2GetLoadedPlugins()) {
        qDebug().noquote() << "Initializing plugin" << loader->fileName();
        splash.showMessage(QApplication::translate("main", "Initializing plugin %1").arg(loader->fileName()), Qt::AlignRight | Qt::AlignBottom, Qt::white);
        WS2Editor::ws2App->processEvents();

        if (WS2Editor::Plugin::IEditorPlugin *editorPlugin = qobject_cast<WS2Editor::Plugin::IEditorPlugin*>(loader->instance())) {
            if (editorPlugin->init()) {
                WS2Editor::ws2GetInitializedPlugins() << loader;
            }
        }
    }

    //Splash message
    splash.showMessage(QApplication::translate("main", "Initializing Stage Editor"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    WS2Editor::UI::StageEditorWindow *w = new WS2Editor::UI::StageEditorWindow();
    w->show();
    splash.finish(w);

    WS2Editor::qAppRunning = true;
    int ret = WS2Editor::ws2App->exec();
    WS2Editor::qAppRunning = false;

    WS2Editor::Resource::ResourceManager::unloadAllResources();

    //Free resources
    WS2Editor::UI::ModelManager::destruct();
    delete w;

    WS2Editor::Command::CommandInterpreter::destroyInstance();
    WS2Editor::ws2Destroy();

    return ret;
}
