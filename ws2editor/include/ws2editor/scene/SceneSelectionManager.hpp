/**
 * @file
 * @brief Header for the SceneSelectionManager class
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_SCENE_SCENESELECTIONMANAGER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_SCENE_SCENESELECTIONMANAGER_HPP

#include "ws2common/scene/SceneNode.hpp"
#include <QVector>
#include <QObject>

namespace WS2Editor {
    namespace Scene {
        class SceneSelectionManager : public QObject {
            Q_OBJECT

            protected:
                QVector<WS2Common::Scene::SceneNode*> selectedObjects;

            public:
                /**
                 * @brief Getter for selectedObjects
                 *
                 * @return A reference to the selectedObjects vector
                 */
                QVector<WS2Common::Scene::SceneNode*>& getSelectedObjects();

                /**
                 * @brief Selects the given object and deselects all other objects
                 *
                 * @param obj The object to be selected
                 */
                void selectOnly(WS2Common::Scene::SceneNode *obj);

                /**
                 * @brief Adds the given obkect to the selection
                 *
                 * @param obj The object to add to the selection
                 */
                void select(WS2Common::Scene::SceneNode *obj);

                /**
                 * @brief Removes the given object from the selection
                 *
                 * @param obj The object to remove from the selection
                 */
                void deselect(WS2Common::Scene::SceneNode *obj);

                /**
                 * @brief Removes all objects from the selection
                 */
                void clearSelection();

            signals:
                /**
                 * @brief Emitted when the selection has been updated
                 *
                 * @param selectedObjects A vector of selected objects A vector of selected objects
                 */
                void onSelectionChanged(QVector<WS2Common::Scene::SceneNode*>& selectedObjects);
        };
    }
}

#endif

