/**
 * @file
 * @brief Header for the Project
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PROJECT_PROJECT_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PROJECT_PROJECT_HPP

#include "ws2editor_export.h"
#include "ws2editor/resource/ResourceScene.hpp"

namespace WS2Editor {
    namespace Project {
        class WS2EDITOR_EXPORT Project {
            protected:
                Resource::ResourceScene *scene = new Resource::ResourceScene();

            public:
                /**
                 * @brief Registers the ResourceScene with ResourceManager
                 */
                Project();

                /**
                 * @brief Destructor for Project
                 *
                 * Frees up memory owned by this object
                 */
                ~Project();

                /**
                 * @brief Getter for WS2Editor::Project::Project::scene
                 *
                 * @return A pointer to the scene
                 */
                Resource::ResourceScene* getScene();

                /**
                 * @brief Imports a 3D model or config file into the project
                 *
                 * @param file The file to import
                 */
                void importFile(QFile &file);

                /**
                 * @brief Imports a 3D model into the project scene
                 *
                 * @param file The file to import
                 */
                void importModel(QFile &file);

                /**
                 * @brief Imports a stage config into the scene
                 *
                 * @param file The file to import
                 */
                void importConfig(QFile &file);
        };
    }
}

#endif

