#include "WS2.hpp"
#include "ui/StageEditorWindow.hpp"
#include "ui/ModelManager.hpp"
#include "resource/ResourceManager.hpp"
#include <QApplication>
#include <QFile>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
    //Set default format
    QSurfaceFormat fmt;

    //Use core profile for OpenGL 3.3
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);

    fmt.setSamples(4); //4x MSAA
    fmt.setSwapInterval(0); //VSync wrecks havok with the framerate in Qt

    QSurfaceFormat::setDefaultFormat(fmt);

    //Construct the application
    QApplication app(argc, argv);

    QFile styleFile(":/Styles/FlatDark/FlatDark.qss");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    styleFile.close();
    app.setStyleSheet(style);

    WS2::UI::StageEditorWindow w;
    w.show();

    WS2::qAppRunning = true;
    int ret = app.exec();
    WS2::qAppRunning = false;

    WS2::Resource::ResourceManager::unloadAllResources();

    //Free resources
    WS2::UI::ModelManager::destruct();

    return ret;
}
