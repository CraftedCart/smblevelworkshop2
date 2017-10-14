/**
 * @file
 * @brief Header for the WS2Common::Scene::SceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SCENENODE_HPP

#include <glm/glm.hpp>
#include <QVector>

namespace WS2Common {
    namespace Scene {
        /**
         * @brief A node in a scene graph
         *
         * A node contains a transform, as well as various children which inherit the transform.
         */
        class SceneNode {
            protected:
                QString name;

                QVector<SceneNode*> children;
                SceneNode *parent;

                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

            public:
                /**
                 * @brief Constructs a new SceneNode with the name specified
                 *
                 * @param name the name of the new SceneNode
                 */
                SceneNode(const QString name);

                /**
                 * @brief Deletes all children
                 */
                virtual ~SceneNode();

                /**
                 * @brief Getter for WS2::Scene::SceneNode::name
                 *
                 * @return The name of this node
                 */
                const QString getName() const;

                /**
                 * @brief Setter for WS2::Scene::SceneNode::name
                 *
                 * @param name The new name to set for the node
                 */
                void setName(const QString name);

                /**
                 * @brief Getter for WS2::Scene::SceneNode::children
                 *
                 * @return A reference to the children QVector
                 */
                QVector<SceneNode*>& getChildren();

                /**
                 * @brief Returns a child node by it's index position in the children QVector
                 *
                 * @param index The index of the child to retrieve
                 *
                 * @return A pointer to the child node at index
                 */
                SceneNode* getChildByIndex(int index);

                /**
                 * @brief Returns a child node with the given name
                 *
                 * @param name The name of the child node to find
                 *
                 * @return A pointer to the child node with the name specified, or nullptr if it doesn't exist
                 */
                SceneNode* getChildByName(QString name);

                /**
                 * @brief Gets the index of this scene node in its parent's children vector
                 *
                 * @return The index of this child within the parent's children QVector, or 0 if there is no parent
                 */
                int getIndex();

                /**
                 * @brief Adds a child node to this node
                 *
                 * @param child The child node to add
                 */
                virtual void addChild(SceneNode *child);

                /**
                 * @brief Gets the number of children that belong to this node
                 *
                 * @return The number of children that belong to this node
                 */
                int getChildCount();

                /**
                 * @brief Getter for WS2::Scene::SceneNode::parent
                 *
                 * @return This node's parent
                 */
                SceneNode* getParent();

                /**
                 * @brief Setter for WS2::Scene::SceneNode::parent
                 *
                 * @param parent The parent to set
                 */
                virtual void setParent(SceneNode *parent);

                /**
                 * @brief Getter for position
                 *
                 * @return The position of the node relative to its parent
                 */
                glm::vec3 getPosition() const;

                /**
                 * @brief Setter for position
                 *
                 * @param position The new position to set
                 */
                void setPosition(const glm::vec3 position);

                /**
                 * @brief Getter for rotation
                 *
                 * @return The rotation of the node relative to its parent
                 */
                glm::vec3 getRotation() const;

                /**
                 * @brief Setter for rotation
                 *
                 * @param rotation The new rotation to set
                 */
                void setRotation(const glm::vec3 rotation);

                /**
                 * @brief Getter for scale
                 *
                 * @return The position of the node relative to its parent
                 */
                glm::vec3 getScale() const;

                /**
                 * @brief Setter for scale
                 *
                 * @param scale The new scale to set
                 */
                void setScale(const glm::vec3 scale);
        };
    }
}

#endif

