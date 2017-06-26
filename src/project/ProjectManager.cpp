#include "project/ProjectManager.hpp"

namespace WS2 {
    namespace Project {
        namespace ProjectManager {
            namespace ProjectManagerInternal {
                //Define the extern variables
                Project *activeProject = new Project();
            }

            Project* getActiveProject() {
                return ProjectManagerInternal::activeProject;
            }
        }
    }
}

