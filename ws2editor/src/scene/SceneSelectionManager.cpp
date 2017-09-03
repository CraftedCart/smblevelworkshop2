#include "scene/SceneSelectionManager.hpp"

namespace WS2Editor {
    namespace Scene {
        QVector<WS2Common::Scene::SceneNode*>& SceneSelectionManager::getSelectedObjects() {
            return selectedObjects;
        }

        void SceneSelectionManager::selectOnly(WS2Common::Scene::SceneNode *obj) {
            selectedObjects.clear();
            selectedObjects.append(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::select(WS2Common::Scene::SceneNode *obj) {
            selectedObjects.append(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::deselect(WS2Common::Scene::SceneNode *obj) {
            selectedObjects.removeOne(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::clearSelection() {
            selectedObjects.clear();

            emit onSelectionChanged(selectedObjects);
        }
    }
}

