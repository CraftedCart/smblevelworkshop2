#include "scene/SceneSelectionManager.hpp"

namespace WS2 {
    namespace Scene {
        QVector<SceneNode*>& SceneSelectionManager::getSelectedObjects() {
            return selectedObjects;
        }

        void SceneSelectionManager::selectOnly(SceneNode *obj) {
            selectedObjects.clear();
            selectedObjects.append(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::select(SceneNode *obj) {
            selectedObjects.append(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::deselect(SceneNode *obj) {
            selectedObjects.removeOne(obj);

            emit onSelectionChanged(selectedObjects);
        }

        void SceneSelectionManager::clearSelection() {
            selectedObjects.clear();

            emit onSelectionChanged(selectedObjects);
        }
    }
}

