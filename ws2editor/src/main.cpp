#include "WS2.hpp"
#include "ui/StageEditorWindow.hpp"
#include "ui/ModelManager.hpp"
#include "resource/ResourceManager.hpp"
#include "MessageHandler.hpp"
#include <QFile>
#include <QSurfaceFormat>
#include <QSplashScreen>

int main(int argc, char *argv[]) {
    qInstallMessageHandler(WS2Common::messageHandler);

    //Init WS2
    WS2Editor::ws2Init(argc, argv);

    //Splash screen
    QPixmap pixmap(":/Workshop2/Images/banner.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(WS2Editor::ws2App->tr("Initializing WS2"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    splash.showMessage(WS2Editor::ws2App->tr("Setting default OpenGL format"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
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
    splash.showMessage(WS2Editor::ws2App->tr("Setting style"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    WS2Editor::ws2App->processEvents();

    QFile styleFile(":/Styles/FlatDark/FlatDark.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    styleFile.close();
    WS2Editor::ws2App->setStyleSheet(style);

    //Splash message
    splash.showMessage(WS2Editor::ws2App->tr("Initializing Stage Editor"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
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

    WS2Editor::ws2Destroy();

    return ret;
}
