#include "ws2editor/project/Project.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2common/resource/ResourceMesh.hpp"
#include <QCoreApplication>

namespace WS2Editor {
    namespace Project {
        Project::Project() {
            scene->setId(Resource::ResourceManager::generateUniqueId(QCoreApplication::translate("Project", "Scene")));
            Resource::ResourceManager::addResource(scene);
        }

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
            using namespace WS2Common::Resource;
            using namespace WS2Editor::Resource;

            const QVector<ResourceMesh*> newMeshes = ResourceManager::addModel(file, scene->isLoaded());
            scene->addModel(newMeshes);
        }

        void Project::importConfig(QFile &file) {
            //TODO
        }
    }
}

