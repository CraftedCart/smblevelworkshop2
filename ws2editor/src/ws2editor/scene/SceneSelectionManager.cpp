#include "ws2editor/scene/SceneSelectionManager.hpp"
#include <QDebug>

namespace WS2Editor {
    namespace Scene {
        using namespace WS2Common::Scene;

        QVector<SceneNode*>& SceneSelectionManager::getSelectedObjects() {
            return selectedObjects;
        }

        QVector<SceneNode*> SceneSelectionManager::getTopmostSelectedObjects() {
            QVector<SceneNode*> v;

            //Iterate over all selected objects and check if any of their ancestors are selected
            for (SceneNode *node : selectedObjects) {
                SceneNode *topmostSelectedNode = node;

                //Find any selected ancestors
                SceneNode *currentSearchedNode = node;
                while (currentSearchedNode->getParent() != nullptr) {
                    currentSearchedNode = currentSearchedNode->getParent();

                    if (isSelected(currentSearchedNode)) {
                        //We found a selected ancestor node
                        topmostSelectedNode = currentSearchedNode;
                    }
                }

                //Add the topmost node to the vector if it's not already in there
                if (!v.contains(topmostSelectedNode)) v.append(topmostSelectedNode);
            }

            return v;
        }

        bool SceneSelectionManager::isSelected(SceneNode *node) const {
            return selectedObjects.contains(node);
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

        void SceneSelectionManager::toggleSelect(SceneNode *obj) {
            if (isSelected(obj)) {
                deselect(obj);
            } else {
                select(obj);
            }
        }

        void SceneSelectionManager::clearSelection() {
            selectedObjects.clear();

            emit onSelectionChanged(selectedObjects);
        }
    }
}

