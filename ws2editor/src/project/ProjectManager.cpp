#include "project/ProjectManager.hpp"

namespace WS2Editor {
    namespace Project {
        namespace ProjectManager {
            namespace ProjectManagerInternal {
                //Define the extern variables
                Project *activeProject = new Project();
            }

            Project* getActiveProject() {
                return ProjectManagerInternal::activeProject;
            }

            void newProject() {
                ProjectManagerInternal::activeProject = new Project();
            }
        }
    }
}

