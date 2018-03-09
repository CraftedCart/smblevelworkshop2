#include "ws2editor/WS2Editor.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/resource/ResourceEditorTexture.hpp"
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
            QVector<WS2Common::Resource::AbstractResource*>& getResources() {
                static QVector<WS2Common::Resource::AbstractResource*> resources;
                return resources;
            }

            void addResource(WS2Common::Resource::AbstractResource *res) {
                getResources().append(res);
                if (qAppRunning) UI::ModelManager::modelResources->onResourceAdded();
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

            /**
             * @throws IOException When failing to read the file
             * @throws RuntimeException When Assimp fails to generate an aiScene
             */
            QVector<ResourceEditorMesh*> addModel(QFile &file, bool shouldLoad) {
                QVector<WS2Common::Resource::ResourceMesh*> meshVec = WS2Common::Model::ModelLoader::loadModel(file, &getResources());
                QVector<void*> toDelete;

                //Convert all the ResourceMeshes to ResourceEditorMeshes, and load them if requested
                //Also convert all the ResourceTextures to ResourceEditorTextures in each MeshSegment
                QVector<ResourceEditorMesh*> vec(meshVec.size());
                int i = 0;
                foreach(WS2Common::Resource::ResourceMesh *mesh, meshVec) {
                    vec[i] = new ResourceEditorMesh(*mesh);

                    foreach (WS2Common::Model::MeshSegment *segment, vec[i]->getMeshSegments()) {
                        QVector<WS2Common::Resource::ResourceTexture*> texVec(segment->getTextures().size());

                        int j = 0;
                        foreach (WS2Common::Resource::ResourceTexture *tex, segment->getTextures()) {
                            if (dynamic_cast<ResourceEditorTexture*>(tex) == nullptr) {
                                if (getResources().indexOf(tex) != -1) {
                                    texVec[j] = new ResourceEditorTexture(*tex);
                                    getResources().replace(getResources().indexOf(tex), texVec[j]);
                                    if (toDelete.indexOf(tex) == -1) toDelete.append(tex);
                                } else {
                                    texVec[j] = getResourceFromFilePath<WS2Common::Resource::ResourceTexture*>(*tex->getFirstFilePath());
                                }
                            } else {
                                texVec[j] = tex;
                            }

                            j++;
                        }

                        //Remove all the deleted old textures, and replace them with the editor textures
                        segment->getTextures().clear();
                        segment->getTextures().append(texVec);
                    }

                    if (shouldLoad) vec[i]->load();
                    getResources().replace(getResources().indexOf(mesh), vec[i]);
                    delete mesh;

                    i++;
                }

                //Delete all old pre-conversion guff
                qDeleteAll(toDelete);

                return vec;
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

