/**
 * @file
 * @brief Header for the WS2Common::Scene::SceneNode class
 */

#ifndef SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SCENENODE_HPP
#define SMBLEVELWORKSHOP2_WS2COMMON_SCENE_SCENENODE_HPP

#include "ws2common/animation/TransformAnimation.hpp"
#include "ws2common/EnumAnimationSeesawType.hpp"
#include "ws2common/Transform.hpp"
#include <glm/glm.hpp>
#include <QVector>
#include <QXmlStreamWriter>
#include <QUuid>

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

                /**
                 * @brief A unique identifier for this node
                 *
                 * Guaranteed to be 99.99% unique!
                 *
                 * @todo Check for collisions... maybe... It would likely be a waste of time tbh
                 */
                QUuid uuid = QUuid::createUuid();

                QVector<SceneNode*> children;
                SceneNode *parent;

                glm::vec3 originPosition = glm::vec3(0.0f, 0.0f, 0.0f);
                glm::vec3 originRotation = glm::vec3(0.0f, 0.0f, 0.0f);

                Transform transform;

                glm::vec3 conveyorSpeed = glm::vec3(0.0f, 0.0f, 0.0f);

                Animation::TransformAnimation *animation = nullptr;
                unsigned int animationGroupId = 0;
                EnumAnimationSeesawType animationSeesawType = ANIMATION;

                float seesawSensitivity = 0.0f;
                float seesawResetStiffness = 0.0f;
                float seesawRotationBounds = 0.0f;

            protected:
                /**
                 * @brief Serializes the data stored in this node to an XML format
                 *
                 * @note When overriding this, you must call the super
                 *
                 * @param sT he XML stream writer to write to
                 */
                virtual void serializeNodeDataXml(QXmlStreamWriter &s) const;

                /**
                 * @brief This returns the identifier used to serialize this node class
                 *
                 * This should be just the class name in camelCase
                 *
                 * @return The identifier used to serialize this node class
                 */
                virtual const QString getSerializableName() const;

            public:
                SceneNode() = default;

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
                 * @brief Serializes the data stored in this node to an XML format
                 *
                 * @param stream The XML stream writer to write to
                 */
                void serializeXml(QXmlStreamWriter &stream) const;

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
                 * @brief Getter for uuid
                 *
                 * @return The uuid of this node
                 */
                const QUuid getUuid() const;

                /**
                 * @brief Setter for uuid
                 *
                 * @param uuid The new uuid to set for the node
                 */
                void setUuid(const QUuid uuid);

                /**
                 * @brief Getter for WS2::Scene::SceneNode::children
                 *
                 * @return A reference to the children QVector
                 */
                QVector<SceneNode*>& getChildren();

                /**
                 * @brief Const getter for WS2::Scene::SceneNode::children
                 *
                 * @return A reference to the children QVector
                 */
                const QVector<SceneNode*>& getChildren() const;

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
                 * @brief Removes a child node from this node and optionally deletes it
                 *
                 * @param child The child node to remove and delete
                 * @param shouldDelete Whether the object should be deleted
                 *
                 * @return The number of nodes removed
                 */
                virtual int removeChild(SceneNode *child, bool shouldDelete = true);

                /**
                 * @brief Removes this node from its parent - this will delete it
                 */
                virtual void removeFromParent();

                /**
                 * @brief Gets the number of children that belong to this node
                 *
                 * @return The number of children that belong to this node
                 */
                int getChildCount() const;

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
                 * @brief Getter for originPosition
                 *
                 * @return The origin point of this node
                 */
                glm::vec3 getOriginPosition() const;

                /**
                 * @brief Setter for originPosition
                 *
                 * @param position The new origin point to set
                 */
                void setOriginPosition(const glm::vec3 originPosition);

                /**
                 * @brief Getter for originRotation
                 *
                 * @return The origin point of this node
                 */
                glm::vec3 getOriginRotation() const;

                /**
                 * @brief Setter for originRotation
                 *
                 * @param position The new origin point to set
                 */
                void setOriginRotation(const glm::vec3 originRotationPosition);

                /**
                 * @brief Getter for transform
                 *
                 * @return The transform of the node relative to its parent
                 */
                const Transform& getTransform() const;

                /**
                 * @brief Const getter for transform
                 *
                 * @return The transform of the node relative to its parent
                 */
                Transform& getTransform();

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

                /**
                 * @brief Getter for conveyorSpeed
                 *
                 * @return The velocity of the conveyor-effect this item group should apply to the player ball
                 */
                glm::vec3 getConveyorSpeed() const;

                /**
                 * @brief Setter for conveyorSpeed
                 *
                 * @param conveyorSpeed The new velocity of the conveyor-effect this item group should apply to the player ball
                 */
                void setConveyorSpeed(const glm::vec3 conveyorSpeed);

                /**
                 * @brief Setter for transformAnimation
                 */
                void setTransformAnimation(Animation::TransformAnimation *anim);

                /**
                 * @brief Removes animation data from this node
                 */
                void removeTransformAnimation();

                /**
                 * @brief Getter for animation
                 *
                 * @return A pointer to the animation data if it exists, nullptr otherwise
                 */
                Animation::TransformAnimation* getTransformAnimation();

                /**
                 * @brief Const getter for animation
                 *
                 * @return A pointer to the animation data if it exists, nullptr otherwise
                 */
                const Animation::TransformAnimation* getTransformAnimation() const;

                /**
                 * @brief Setter for animationGroupId
                 *
                 * @param animationGroupId The new ID to set
                 */
                void setAnimationGroupId(unsigned int animationGroupId);

                /**
                 * @brief Getter for animationGroupId
                 *
                 * @return The numberical group that this animation is a part of
                 */
                unsigned int getAnimationGroupId() const;

                /**
                 * @brief Getter for animationSeesawType
                 *
                 * @return Whether this scene node is marked as animatable or as a seesaw
                 */
                EnumAnimationSeesawType getAnimationSeesawType() const;

                /**
                 * @brief Setter for animationSeesawType
                 *
                 * @param type The type to set
                 */
                void setAnimationSeesawType(EnumAnimationSeesawType type);

                /**
                 * @brief Getter for seesawSensitivity
                 *
                 * @return The seesaw sensitivity (Higher is more sensitive, negative makes the seesaw reversed)
                 */
                float getSeesawSensitivity() const;

                /**
                 * @brief Setter for seesawSensitivity
                 *
                 * @param value The new seesaw sensitivity (Higher is more sensitive, negative makes the seesaw reversed)
                 */
                void setSeesawSensitivity(float value);

                /**
                 * @brief Getter for seesawResetStiffness
                 *
                 * @return The seesaw reset stiffness (Lower is looser)
                 */
                float getSeesawResetStiffness() const;

                /**
                 * @brief Setter for seesawResetStiffness
                 *
                 * @param value The new seesaw reset stiffness (Lower is looser)
                 */
                void setSeesawResetStiffness(float value);

                /**
                 * @brief Getter for seesawRotationBounds
                 *
                 * @return The seesaw bounds of rotation (Higher is less)
                 */
                float getSeesawRotationBounds() const;

                /**
                 * @brief Setter for seesawRotationBounds
                 *
                 * @param value The new seesaw seesaw bounds of rotation (Higher is less)
                 */
                void setSeesawRotationBounds(float value);
        };
    }
}

#endif

