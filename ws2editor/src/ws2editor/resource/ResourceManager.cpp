#include "ws2editor/WS2EditorInstance.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2common/MathUtils.hpp"
#include "ws2common/exception/IOException.hpp"
#include "ws2common/exception/ModelLoadingException.hpp"
#include "ws2common/model/ModelLoader.hpp"
#include "ws2common/WS2Common.hpp"
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <QDebug>
#include <climits>

namespace WS2Editor {
    namespace Resource {
        namespace ResourceManager {
            using namespace WS2Common::Resource;

            //Define externs
            QMutex resourcesMutex;

            QVector<WS2Common::Resource::AbstractResource*>& getResources() {
                static QVector<WS2Common::Resource::AbstractResource*> resources;
                return resources;
            }

            void addResource(WS2Common::Resource::AbstractResource *res) {
                resourcesMutex.lock();
                getResources().append(res);
                resourcesMutex.unlock();

                if (WS2EditorInstance::getInstance() != nullptr) UI::ModelManager::modelResources->onResourceAdded();
            }

            void unloadAllResources() {
                for (int i = 0; i < getResources().size(); i++) {
                    WS2Common::Resource::AbstractResource *res = getResources().at(i);
                    if (res->isLoaded()) {
                        qDebug() << "Unloading:" << res->getId();
                        res->unload();
                    }
                }
            }

            QVector<ResourceMesh*> addModel(QFile &file, bool shouldLoad) {
                using namespace WS2Common::Resource;
                using namespace WS2Common::Model;

                QVector<ResourceMesh*> meshVec = ModelLoader::loadModel(file, &getResources(), &resourcesMutex);

                resourcesMutex.lock();
                foreach(ResourceMesh *mesh, meshVec) {
                    getResources().append(mesh);
                }
                resourcesMutex.unlock();

                return meshVec;
            }

            /**
             * @throws WS2Editor::Exception::RuntimeException When there are UINT_MAX taken names
             */
            QString generateUniqueId(QString prefix) {
                //Create and populate vector of taken names
                int size = getResources().size();
                QVector<QString> takenNames(size);

                for (int i = 0; i < size; i++) {
                    takenNames[i] = getResources().at(i)->getId();
                }

                //Keep generating a new Id with an increacing suffix number until one isn't taken
                unsigned int suffix = 0;
                QString str; //No need to allocate str on each iteration of the loop

                while (suffix < UINT_MAX) {
                    str = QString("%1%2").arg(prefix).arg(suffix);
                    if (!takenNames.contains(str)) return str;

                    suffix++;
                }

                //Ok so someone had the idea of having UINT_MAX taken names
                throw WS2Common::Exception::RuntimeException("There are UINT_MAX or more matching names in "
                        "generateUniqueId(QString prefix)");
            }
        }
    }
}

