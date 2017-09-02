#include "scene/SceneSelectionManager.hpp"

namespace WS2 {
    namespace Scene {
        QVector<Common::Scene::SceneNode*>& SceneSelectionManager::getSelectedObjects() {
            return selectedObjects;
        }

        void SceneSelectionManager::selectOnly(Common::Scene::SceneNode *obj) {
            selectedObjects.clear();
            selectedObjects.append(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::select(Common::Scene::SceneNode *obj) {
            selectedObjects.append(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::deselect(Common::Scene::SceneNode *obj) {
            selectedObjects.removeOne(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::clearSelection() {
            selectedObjects.clear();

            emit onSelectionChanged(selectedObjects);
        }
    }
}

