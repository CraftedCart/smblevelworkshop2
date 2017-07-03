#include "ws2.hpp"
#include "StageEditorWindow.hpp"
#include "ui/ModelManager.hpp"
#include "project/ProjectManager.hpp"
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

    //Create a new project
    //WS2::Project::ProjectManager::newProject();

    WS2::qAppRunning = true;
    WS2::StageEditorWindow w;
    w.show();

    int ret = app.exec();

    //Free resources
    WS2::UI::ModelManager::destruct();

    return ret;
}
