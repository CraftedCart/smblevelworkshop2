#include "WS2.hpp"

namespace WS2Editor {
    //Define extern members
    Task::TaskManager *ws2TaskManager;
    QApplication *ws2App;
    bool qAppRunning = false;
    std::mt19937 *randGen;

    void ws2Init(int &argc, char *argv[]) {
        ws2App = new QApplication(argc, argv);
        ws2TaskManager = new Task::TaskManager();

        std::random_device rd;
        randGen = new std::mt19937(rd());
    }

    void ws2Destroy() {
        delete ws2TaskManager;
        delete ws2App;
        delete randGen;
    }
}

