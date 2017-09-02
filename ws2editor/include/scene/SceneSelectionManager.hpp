/**
 * @file
 * @brief Header for the SceneSelectionManager class
 */

#ifndef SMBLEVELWORKSHOP2_SCENE_SCENESELECTIONMANAGER_HPP
#define SMBLEVELWORKSHOP2_SCENE_SCENESELECTIONMANAGER_HPP

#include "scene/SceneNode.hpp"
#include <QVector>
#include <QObject>

namespace WS2 {
    namespace Scene {
        class SceneSelectionManager : public QObject {
            Q_OBJECT

            protected:
                QVector<Common::Scene::SceneNode*> selectedObjects;

            public:
                /**
                 * @brief Getter for selectedObjects
                 *
                 * @return A reference to the selectedObjects vector
                 */
                QVector<Common::Scene::SceneNode*>& getSelectedObjects();

                /**
                 * @brief Selects the given object and deselects all other objects
                 *
                 * @param obj The object to be selected
                 */
                void selectOnly(Common::Scene::SceneNode *obj);

                /**
                 * @brief Adds the given obkect to the selection
                 *
                 * @param obj The object to add to the selection
                 */
                void select(Common::Scene::SceneNode *obj);

                /**
                 * @brief Removes the given object from the selection
                 *
                 * @param obj The object to remove from the selection
                 */
                void deselect(Common::Scene::SceneNode *obj);

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
                void onSelectionChanged(QVector<Common::Scene::SceneNode*>& selectedObjects);
        };
    }
}

#endif

