#include "WS2.hpp"
#include "ui/StageEditorWindow.hpp"
#include "ui/ModelManager.hpp"
#include "resource/ResourceManager.hpp"
#include <QApplication>
#include <QFile>
#include <QSurfaceFormat>
#include <QSplashScreen>

int main(int argc, char *argv[]) {
    //Construct the application
    QApplication app(argc, argv);

    //Splash screen
    QPixmap pixmap(":/Workshop2/Images/banner.png");
    QSplashScreen splash(pixmap);
    splash.show();
    splash.showMessage(app.tr("Initializing WS2"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    app.processEvents();

    //Init WS2
    WS2::ws2Init();

    splash.showMessage(app.tr("Setting default OpenGL format"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    app.processEvents();

    //Set default format
    QSurfaceFormat fmt;

    //Use core profile for OpenGL 3.3
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);

    fmt.setSamples(4); //4x MSAA
    fmt.setSwapInterval(0); //VSync wrecks havok with the framerate in Qt

    QSurfaceFormat::setDefaultFormat(fmt);

    //Splash message
    splash.showMessage(app.tr("Setting style"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    app.processEvents();

    QFile styleFile(":/Styles/FlatDark/FlatDark.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    styleFile.close();
    app.setStyleSheet(style);

    //Splash message
    splash.showMessage(app.tr("Initializing Stage Editor"), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    app.processEvents();

    WS2::UI::StageEditorWindow w;
    w.show();
    splash.finish(&w);

    WS2::qAppRunning = true;
    int ret = app.exec();
    WS2::qAppRunning = false;

    WS2::Resource::ResourceManager::unloadAllResources();

    //Free resources
    WS2::UI::ModelManager::destruct();

    return ret;
}
