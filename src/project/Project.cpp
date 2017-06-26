#include "project/Project.hpp"

namespace WS2 {
    namespace Project {
        Project::~Project() {
            delete scene;
        }

        Resource::ResourceScene* Project::getScene() {
            return scene;
        }

        void Project::importFile(QFile &file) {
            const QString lowerFileName = file.fileName().toLower();
            if (
                    lowerFileName.endsWith(".txt") ||
                    (lowerFileName.endsWith(".xml") && !lowerFileName.endsWith(".mesh.xml"))
               ) {
                //It's a stage config
                importConfig(file);
            } else {
                //It's a 3D model
                importModel(file);
            }
        }

        void Project::importModel(QFile &file) {
            scene->addModel(file);
        }

        void Project::importConfig(QFile &file) {
            //TODO
        }
    }
}

