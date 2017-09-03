/**
 * @file
 * @brief Header for the WS2Editor::Project::ProjectManager namespace
 */

#ifndef SMBLEVELWORKSHOP2_PROJECT_PROJECTMANAGER_HPP
#define SMBLEVELWORKSHOP2_PROJECT_PROJECTMANAGER_HPP

#include "project/Project.hpp"

namespace WS2Editor {
    namespace Project {
        namespace ProjectManager {
            /**
             * @brief Internal members for ProjectManager. It is recommended to use use the functions in ProjectManager
             *        over accessing the internals directly.
             */
            namespace ProjectManagerInternal {
                extern Project *activeProject;
            }

            /**
             * @brief Getter for WS2Editor::Project::ProjectManager::ProjectManagerInternal::activeProject
             *
             * @return A pointer to the active project
             */
            Project* getActiveProject();

            /**
             * @brief Creates a new empty project, and sets it as the active project
             */
            void newProject();
        }
    }
}

#endif

