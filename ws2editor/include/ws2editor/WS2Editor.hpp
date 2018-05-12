/**
 * @file
 * @brief Header for global WS2 members
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WS2EDITOR_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WS2EDITOR_HPP

#include "task/TaskManager.hpp"
#include <QApplication>
#include <QPluginLoader>

/**
 * @brief Namespace where all SMB Level Workshop 2 Editor code resides in
 *
 * WS2 is short for Workshop 2
 */
namespace WS2Editor {
    extern Task::TaskManager *ws2TaskManager;
    extern QApplication *ws2App;
    extern bool qAppRunning;

    //Prefixed with ws2 to avoid confusion with other functions that are within the WS2Editor namespace
    void ws2Init(int &argc, char *argv[]);
    void ws2Destroy();

    /**
     * @brief Gets plugins that QPluginLoader managed to load (But not necessarily initialize)
     *
     * @brief Plugins that QPluginLoader managed to load (But not necessarily initialize)
     */
    QVector<QPluginLoader*>& ws2GetLoadedPlugins();

    /**
     * @brief Gets plugins that have successfully been loaded and initialized
     *
     * @return Plugins that have successfully been loaded and initialized
     */
    QVector<QPluginLoader*>& ws2GetInitializedPlugins();

    /**
     * @brief Gets plugins that failed to load
     *
     * @return Plugins that failed to load
     */
    QVector<QPluginLoader*>& ws2GetFailedPlugins();
}

#endif

