#include "gizmosplugin/RenderGlobals.hpp"
#include "ws2common/model/ModelLoader.hpp"
#include <QFile>

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        namespace RenderGlobals {
            //Define externs
            WS2Common::Model::MeshSegment *lineMeshSegment;
            QVector<WS2Common::Resource::ResourceMesh*> gizmoConeMesh;
            QVector<WS2Common::Resource::ResourceMesh*> cubeMesh;

            void loadDefaultModels() {
                using namespace WS2Common::Model;
                using namespace WS2Common::Resource;

                Vertex lineVertA;
                lineVertA.position = glm::vec3(0.0f, 0.0f, 0.0f);
                Vertex lineVertB;
                lineVertB.position = glm::vec3(0.0f, 1.0f, 0.0f);
                lineMeshSegment = new MeshSegment(
                        QVector<Vertex> {lineVertA, lineVertB},
                        QVector<unsigned int> {0, 1},
                        QVector<ResourceTexture*> {}
                        );

                QFile gizmoConeFile(":/GizmosPlugin/Models/gizmoCone.fbx");
                gizmoConeMesh = ModelLoader::loadModel(gizmoConeFile);

                QFile cubeFile(":/GizmosPlugin/Models/cube.fbx");
                cubeMesh = ModelLoader::loadModel(cubeFile);
            }

            void unloadDefaultModels() {
                delete lineMeshSegment;
                qDeleteAll(gizmoConeMesh);
                qDeleteAll(cubeMesh);
            }
        }
    }
}

