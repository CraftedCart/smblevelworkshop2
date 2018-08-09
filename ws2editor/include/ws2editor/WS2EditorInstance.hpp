/**
 * @file
 * @brief Header for the WS2EditorInstance class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_WS2EDITORINSTANCE_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_WS2EDITORINSTANCE_HPP

#include "ws2editor_export.h"
#include "ws2editor/task/TaskManager.hpp"
#include "ws2editor/IExportProvider.hpp"
#include "ui/StageEditorWindow.hpp"
#include <QApplication>
#include <QPluginLoader>

/**
 * @brief Namespace where all SMB Level Workshop 2 Editor code resides in
 *
 * WS2 is short for Workshop 2
 */
namespace WS2Editor {
    class WS2EDITOR_EXPORT WS2EditorInstance : public QObject {
        Q_OBJECT

        private:
            /**
             * @brief The singleton instance of this object
             */
            WS2EDITOR_EXPORT static WS2EditorInstance *instance;

            QApplication *app;
            Task::TaskManager *taskManager;

            QVector<IExportProvider*> exportProviders;

        private:
            WS2EditorInstance(int &argc, char *argv[]);
            ~WS2EditorInstance();

        public:
            /**
             * @brief Creates the singleton instance of this object
             *
             * Command line args are passed to the QApplication constructor
             *
             * @param argc The number of command line arguments
             * @param argv[] The command line arguments
             */
            WS2EDITOR_EXPORT static void createInstance(int &argc, char *argv[]);

            /**
             * @brief Getter for the singleton instance
             *
             * @return The singleton instance if it has been created
             */
            WS2EDITOR_EXPORT static WS2EditorInstance* getInstance();

            /**
             * @brief Deletes the singleton instance
             */
            WS2EDITOR_EXPORT static void destroyInstance();

            /**
             * @brief Getter for the QApplication
             *
             * Alternatively, you could use the global qApp macro provided by Qt
             *
             * @return The QApplication
             */
            QApplication* getApp();

            /**
             * @brief Getter for the task manager
             *
             * @return The task manager
             */
            Task::TaskManager* getTaskManager();

            /**
             * @brief Gets plugins that QPluginLoader managed to load (But not necessarily initialize)
             *
             * @brief Plugins that QPluginLoader managed to load (But not necessarily initialize)
             */
            QVector<QPluginLoader*>& getLoadedPlugins();

            /**
             * @brief Gets plugins that have successfully been loaded and initialized
             *
             * @return Plugins that have successfully been loaded and initialized
             */
            QVector<QPluginLoader*>& getInitializedPlugins();

            /**
             * @brief Gets plugins that failed to load
             *
             * @return Plugins that failed to load
             */
            QVector<QPluginLoader*>& getFailedPlugins();

            /**
             * @brief Registers an export provider
             *
             * When the user requests to export a file they will be presented with a list of export providers to select
             * from.
             *
             * When the provider is deleted it will automatically be unregistered
             *
             * @param provider The export provider to register
             */
            void registerExportProvider(IExportProvider *provider);

            /**
             * @brief Getter for exportProviders
             *
             * @return A vector of all export providers, which are used to generate data for exporting files
             */
            QVector<IExportProvider*>& getExportProviders();

            /**
             * @brief Getter for exportProviders - const edition
             *
             * @return A vector of all export providers, which are used to generate data for exporting files
             */
            const QVector<IExportProvider*>& getExportProviders() const;

            /**
             * @brief Calls the exec function on the QApplcation, entering the event loop and staying in there until
             *        the application requests to quit.
             *
             * @return The return code for the application
             */
            int execApp();

        signals:
            /**
             * @brief Emitted after the StageEditorWindow has been constructed
             *
             * @param w A reference to the stage editor window
             */
            void onStageEditorWindowConstructed(UI::StageEditorWindow &w);
    };
}

#endif

