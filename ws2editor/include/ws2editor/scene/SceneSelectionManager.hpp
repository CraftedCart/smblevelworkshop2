/**
 * @file
 * @brief Header for the SceneSelectionManager class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_SCENE_SCENESELECTIONMANAGER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_SCENE_SCENESELECTIONMANAGER_HPP

#include "ws2editor_export.h"
#include "ws2common/scene/SceneNode.hpp"
#include <QVector>
#include <QObject>

namespace WS2Editor {
    namespace Scene {
        class WS2EDITOR_EXPORT SceneSelectionManager : public QObject {
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
                 * @brief Returns a vector of selected objects, discarding selected children who have an ancestor node
                 *        selected (Not necessarily a direct parent)
                 *
                 * For example, let's say we had a scenegraph such as the following, with items in square brackets
                 * selected
                 *
                 * - root
                 *     - [Group A]
                 *         - Subgroup A
                 *             - [Object A]
                 *     - Group B
                 *         - [Object B]
                 *     - Group C
                 *
                 * This would return a vector containing Group A and Object B only
                 *
                 * @return A vector of topmost selected objects
                 */
                QVector<WS2Common::Scene::SceneNode*> getTopmostSelectedObjects();

                /**
                 * @brief Checks if a node is selected
                 *
                 * @param node The node to check
                 *
                 * @return Whether the node is selected or not
                 */
                bool isSelected(WS2Common::Scene::SceneNode *node) const;

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
                 * @brief Selects the object is it isn't selected, otherwise deselects it
                 *
                 * @param obj The object to togle seelction on
                 */
                void toggleSelect(WS2Common::Scene::SceneNode *obj);

                /**
                 * @brief Removes all objects from the selection
                 */
                void clearSelection();

            signals:
                /**
                 * @brief Emitted when the selection has been updated
                 *
                 * @param selectedObjects A vector of selected objects
                 */
                void onSelectionChanged(QVector<WS2Common::Scene::SceneNode*>& selectedObjects);
        };
    }
}

#endif

