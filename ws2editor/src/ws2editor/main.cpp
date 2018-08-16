#include "ws2editor_export.h"
#include "ws2editor/WS2EditorInstance.hpp"
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

namespace WS2Editor {
    WS2EDITOR_EXPORT int ws2editorLaunch(int argc, char *argv[]) {
        using namespace WS2Editor;

        qInstallMessageHandler(WS2Common::messageHandler);

        //Init WS2
        WS2EditorInstance::createInstance(argc, argv);
        WS2EditorInstance *ws2Instance = WS2EditorInstance::getInstance();

        //Load translations
        QTranslator translator;
        if (translator.load(QLocale(), QLatin1String("lang"), QLatin1String("_"), QDir(QCoreApplication::applicationDirPath()).filePath("../share/ws2editor/lang"))) {
            qApp->installTranslator(&translator);
        } else if (translator.load(QLocale(), QLatin1String("lang"), QLatin1String("_"), QCoreApplication::applicationDirPath())) {
            //If the software was never installed after build, the translations will be alongside the executable
            qApp->installTranslator(&translator);
        } else if (translator.load("lang_en_US", QDir(QCoreApplication::applicationDirPath()).filePath("../share/ws2editor/lang"))) {
            //If we can't find a suitable translation, try en_US
            qApp->installTranslator(&translator);
        } else if (translator.load("lang_en_US", QCoreApplication::applicationDirPath())) {
            //If we can't find a suitable translation, try en_US - if not intalled
            qApp->installTranslator(&translator);
        }

        //Init command interpreter
        Command::CommandInterpreter::createInstance();

        //Splash screen
        QPixmap pixmap(":/Workshop2/Images/banner.png");
        QSplashScreen splash(pixmap);
        splash.show();
        splash.showMessage(QApplication::translate("main", "Initializing WS2"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
        qApp->processEvents();

        splash.showMessage(QApplication::translate("main", "Setting default OpenGL format"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
        qApp->processEvents();

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
        qApp->processEvents();

        QFile styleFile(":/Styles/FlatDark/FlatDark.qss");
        styleFile.open(QFile::ReadOnly);
        QString style(styleFile.readAll());
        styleFile.close();
        qApp->setStyleSheet(style);

        //Load plugins
        splash.showMessage(QApplication::translate("main", "Finding plugins"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
        qApp->processEvents();

        qDebug() << "Loading plugins";
        QDir pluginsDir = QDir(QApplication::applicationDirPath());
        pluginsDir.cdUp();
        pluginsDir.cd("share");
        pluginsDir.cd("ws2editor");
        //Try and make the plugins dir if it doesn't exist
        pluginsDir.mkdir("plugins");
        pluginsDir.cd("plugins");

        for (QString fileName : pluginsDir.entryList(QDir::Files)) {
#ifdef Q_OS_LINUX
            if (!fileName.toLower().endsWith(".so")) {
                qDebug().noquote() << "Ignoring" << fileName << "- Filename does not end in .so";
                continue;
            }
#elif defined(Q_OS_DARWIN)
            if (!fileName.toLower().endsWith(".dylib")) {
                qDebug().noquote() << "Ignoring" << fileName << "- Filename does not end in .dylib";
                continue;
            }
#elif defined(Q_OS_WIN)
            if (!fileName.toLower().endsWith(".dll")) {
                qDebug().noquote() << "Ignoring" << fileName << "- Filename does not end in .dll";
                continue;
            }
#endif
            //Otherwise if we're on some unknown OS, we'll just try and load everything

            qDebug().noquote() << "Loading plugin" << fileName;
            splash.showMessage(QApplication::translate("main", "Loading plugin %1").arg(fileName), Qt::AlignRight | Qt::AlignBottom, Qt::white);
            qApp->processEvents();

            QPluginLoader *loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader->instance();

            if (plugin) {
                ws2Instance->getLoadedPlugins() << loader;
            } else {
                qDebug().noquote() << "Failed to load plugin" << fileName << "-" << loader->errorString();
                loader->unload();

                ws2Instance->getFailedPlugins() << loader;
            }
        }

        //Now that we've loaded all the plugins, try to initialize them
        for (QPluginLoader *loader : ws2Instance->getLoadedPlugins()) {
            qDebug().noquote() << "Initializing plugin" << loader->fileName();
            splash.showMessage(QApplication::translate("main", "Initializing plugin %1").arg(loader->fileName()), Qt::AlignRight | Qt::AlignBottom, Qt::white);
            qApp->processEvents();

            if (Plugin::IEditorPlugin *editorPlugin = qobject_cast<Plugin::IEditorPlugin*>(loader->instance())) {
                if (editorPlugin->init()) {
                    ws2Instance->getInitializedPlugins() << loader;
                }
            }
        }

        //Splash message
        splash.showMessage(QApplication::translate("main", "Initializing Stage Editor"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
        qApp->processEvents();

        UI::StageEditorWindow *w = new UI::StageEditorWindow();
        emit ws2Instance->onStageEditorWindowConstructed(*w);
        w->show();
        splash.finish(w);

        //Enter the event loop until we request to quit
        int ret = ws2Instance->execApp();

        WS2Editor::Resource::ResourceManager::unloadAllResources();

        //Free resources
        UI::ModelManager::destruct();
        delete w;

        Command::CommandInterpreter::destroyInstance();
        WS2EditorInstance::destroyInstance();

        return ret;
    }
}

