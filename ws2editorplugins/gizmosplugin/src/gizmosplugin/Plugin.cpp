#include "gizmosplugin/Plugin.hpp"
#include "gizmosplugin/RenderGlobals.hpp"
#include "gizmosplugin/rendering/TranslateGizmoRenderCommand.hpp"
#include "ws2editor/physics/PhysicsUtils.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/ui/ModelManager.hpp"
#include "ws2common/model/ModelLoader.hpp"
#include "ws2common/MathUtils.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <QDebug>

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        using namespace WS2EditorPlugins::GizmosPlugin::Rendering;
        using namespace WS2Editor;
        using namespace WS2Editor::Widget;
        using namespace WS2Editor::Project;
        using namespace WS2Editor::UI;
        using namespace WS2Editor::Physics;
        using namespace WS2Editor::Resource;
        using namespace WS2Common;
        using namespace WS2Common::MathUtils;
        using namespace WS2Common::Scene;

        bool Plugin::init() {
            //Hook into the stage editor window by calling onStageEditorWindowConstructed after it's been created
            connect(WS2EditorInstance::getInstance(), &WS2EditorInstance::onStageEditorWindowConstructed,
                    this, &Plugin::onStageEditorWindowConstructed,
                    Qt::DirectConnection);

            //The plugin initialized successfully, return true
            qInfo() << "WS2Editor GizmosPlugin successfully initialized";
            return true;
        }

        void Plugin::onStageEditorWindowConstructed(UI::StageEditorWindow &w) {
            viewportWidget = w.getViewportWidget();
            renderManager = viewportWidget->getRenderManager();

            //Render manager
            connect(viewportWidget->getRenderManager(), &RenderManager::postInit,
                    this, &Plugin::onRenderManagerPostInit,
                    Qt::DirectConnection);
            connect(viewportWidget->getRenderManager(), &RenderManager::preDestroy,
                    this, &Plugin::onRenderManagerPreDestroy,
                    Qt::DirectConnection);

            //Viewport widget
            connect(viewportWidget, &ViewportWidget::postInitializeGl,
                    this, &Plugin::onViewportWidgetPostInitializeGl,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::onPhysicsObjectSelected,
                    this, &Plugin::onViewportWidgetPhysicsObjectSelected,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::onPhysicsObjectMouseOver,
                    this, &Plugin::onViewportWidgetPhysicsObjectMouseOver,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::onPhysicsObjectMouseOverNothing,
                    this, &Plugin::onViewportWidgetPhysicsObjectMouseOverNothing,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::onMousePressed,
                    this, &Plugin::onViewportWidgetMousePressed,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::onMouseReleased,
                    this, &Plugin::onViewportWidgetMouseReleased,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::postPreDraw,
                    this, &Plugin::onViewportWidgetPostPreDraw,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::postRenderScene,
                    this, &Plugin::updateGizmos,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::preDestroy,
                    this, &Plugin::onViewportWidgetPreDestroy,
                    Qt::DirectConnection);
        }

        void Plugin::onViewportWidgetPreDestroy(WS2Editor::Widget::ViewportWidget &viewportWidget) {
            Q_UNUSED(viewportWidget);

            delete gizmoYPhysics;
            delete gizmoXPhysics;
            delete gizmoZPhysics;
        }

        void Plugin::onViewportWidgetPostInitializeGl(WS2Editor::Widget::ViewportWidget &viewportWidget) {
            Q_UNUSED(viewportWidget);

            Transform t;
            gizmoYPhysics = new PhysicsContainer(nullptr, RenderGlobals::cubeMesh.at(0), t);
            gizmoXPhysics = new PhysicsContainer(nullptr, RenderGlobals::cubeMesh.at(0), t);
            gizmoZPhysics = new PhysicsContainer(nullptr, RenderGlobals::cubeMesh.at(0), t);
        }

        void Plugin::onViewportWidgetPhysicsObjectSelected(
                btCollisionWorld::AllHitsRayResultCallback *rayCallback,
                bool &outHandled
                ) {
            //Check if any of the gizmos were hit, regardless of whether it was the closest objects or not
            //We don't do anything with them apart from set outHandled to true to prevent crashing
            if (PhysicsUtils::doesAlignedObjectArrayContain(
                        rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoYPhysics->getRigidBody())) {
                outHandled = true;
            } else if (PhysicsUtils::doesAlignedObjectArrayContain(
                        rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoXPhysics->getRigidBody())) {
                outHandled = true;
            } else if (PhysicsUtils::doesAlignedObjectArrayContain(
                        rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoZPhysics->getRigidBody())) {
                outHandled = true;
            }
        }

        void Plugin::onViewportWidgetPhysicsObjectMouseOver(
                btCollisionWorld::AllHitsRayResultCallback *rayCallback,
                bool &outHandled
                ) {
                //Check if any of the gizmos were hit, regardless of whether it was the closest objects or not
                //And mark them as highlighted
                if (PhysicsUtils::doesAlignedObjectArrayContain(
                            rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoYPhysics->getRigidBody())) {
                    highlightGizmoY = true;
                    highlightGizmoX = false;
                    highlightGizmoZ = false;
                    outHandled = true;
                } else if (PhysicsUtils::doesAlignedObjectArrayContain(
                            rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoXPhysics->getRigidBody())) {
                    highlightGizmoY = false;
                    highlightGizmoX = true;
                    highlightGizmoZ = false;
                    outHandled = true;
                } else if (PhysicsUtils::doesAlignedObjectArrayContain(
                            rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoZPhysics->getRigidBody())) {
                    highlightGizmoY = false;
                    highlightGizmoX = false;
                    highlightGizmoZ = true;
                    outHandled = true;
                } else {
                    highlightGizmoY = highlightGizmoX = highlightGizmoZ = false;
                }
        }

        void Plugin::onViewportWidgetPhysicsObjectMouseOverNothing() {
            //Don't highlight any gizmos
            highlightGizmoY = highlightGizmoX = highlightGizmoZ = false;
        }

        void Plugin::onViewportWidgetMousePressed(QMouseEvent *event) {
            if (event->button() == Qt::LeftButton) {
                activeGizmoY = highlightGizmoY;
                activeGizmoX = highlightGizmoX;
                activeGizmoZ = highlightGizmoZ;


                if (activeGizmoY || activeGizmoX || activeGizmoZ) {
                    glm::vec4 transformedLocation =
                        viewportWidget->getProjMatrix() * viewportWidget->getViewMatrix() * glm::vec4(gizmoPosition, 1.0f);
                    float zPlane = (glm::vec3(transformedLocation) / transformedLocation.w).z;

                    preGrabPosition = viewportWidget->zPlaneRaycast(
                            glm::vec3(
                                MathUtils::toGlmVec2(QVector2D(viewportWidget->mapFromGlobal(QCursor::pos()))),
                                zPlane
                                )
                            );

                    QVector<SceneNode*> topmostSelected =
                        ProjectManager::getActiveProject()->
                        getScene()->
                        getSelectionManager()->
                        getTopmostSelectedObjects();

                    preGrabTransforms.clear();
                    for (SceneNode *node : topmostSelected) {
                        preGrabTransforms[node] = node->getTransform(); //Copy the initial transforms for each node
                    }
                }
            }
        }

        void Plugin::onViewportWidgetMouseReleased(QMouseEvent *event) {
            if (event->button() == Qt::LeftButton) {
                activeGizmoY = activeGizmoX = activeGizmoZ = false;
            }
        }

        void Plugin::onViewportWidgetPostPreDraw(ViewportWidget &viewportWidget) {
            Q_UNUSED(viewportWidget);

            if (!(activeGizmoY || activeGizmoX || activeGizmoZ)) return;

            glm::vec4 transformedLocation =
                viewportWidget.getProjMatrix() * viewportWidget.getViewMatrix() * glm::vec4(gizmoPosition, 1.0f);
            float zPlane = (glm::vec3(transformedLocation) / transformedLocation.w).z;

            glm::vec3 mousePos = viewportWidget.zPlaneRaycast(
                    glm::vec3(
                        MathUtils::toGlmVec2(QVector2D(viewportWidget.mapFromGlobal(QCursor::pos()))),
                        zPlane
                        )
                    );

            QHashIterator<SceneNode*, Transform> i(preGrabTransforms);
            while (i.hasNext()) {
                i.next();
                //Get the delta position from the start pos to the cursor pos
                glm::vec3 deltaPos = mousePos - preGrabPosition;
                glm::vec3 newPos = i.value().getPosition(); //The new position of the node

                if (activeGizmoY) newPos.y = i.value().getPosition().y + deltaPos.y;
                if (activeGizmoX) newPos.x = i.value().getPosition().x + deltaPos.x;
                if (activeGizmoZ) newPos.z = i.value().getPosition().z + deltaPos.z;

                i.key()->getTransform().setPosition(newPos);
                ModelManager::modelOutliner->nodeModified(i.key());
            }
        }

        void Plugin::onRenderManagerPostInit(RenderManager &renderManager) {
            Q_UNUSED(renderManager);

            RenderGlobals::loadDefaultModels();
        }

        void Plugin::onRenderManagerPreDestroy(RenderManager &renderManager) {
            Q_UNUSED(renderManager);

            RenderGlobals::unloadDefaultModels();

        }

        void Plugin::updateGizmos(ResourceScene &scene) {
            using namespace WS2Editor::Scene;
            using namespace WS2Common::Resource;

            SceneSelectionManager *selectionManager = scene.getSelectionManager();

            if (selectionManager->getSelectedObjects().size() > 0) {
                //Add the collision rigid bodies to the world if they aren't in there already
                if (!isGizmoPhysicsInWorld) {
                    scene.getPhysicsManager()->addRigidBody(gizmoYPhysics->getRigidBody());
                    scene.getPhysicsManager()->addRigidBody(gizmoXPhysics->getRigidBody());
                    scene.getPhysicsManager()->addRigidBody(gizmoZPhysics->getRigidBody());

                    isGizmoPhysicsInWorld = true;
                }

                //Merge all selected objects'/positions AABBs into one
                SceneNode *initialNode = selectionManager->getSelectedObjects().at(0);
                MeshNodeData *initialMeshData = scene.getMeshNodeData(initialNode->getUuid());

                AABB3 aabb;
                if (initialMeshData != nullptr) {
                    aabb = initialMeshData->getAabb();
                    aabb.offsetBy(initialNode->getPosition());
                } else {
                    aabb = AABB3(initialNode->getPosition(), initialNode->getPosition());
                }

                for (SceneNode *node : selectionManager->getSelectedObjects()) {
                    MeshNodeData *meshData = scene.getMeshNodeData(node->getUuid());

                    if (meshData != nullptr) {
                        AABB3 newAabb = meshData->getAabb();
                        newAabb.offsetBy(node->getPosition());
                        aabb.mergeWith(newAabb);
                    } else {
                        aabb.mergeWith(node->getPosition());
                    }
                }

                //Get the middle point of the AABB, and draw the gizmo there
                gizmoPosition = aabb.getCenter();
                renderManager->enqueueRenderCommand(new TranslateGizmoRenderCommand(
                            renderManager,
                            glm::translate(gizmoPosition),
                            viewportWidget->getViewMatrix(),
                            viewportWidget->getProjMatrix(),
                            viewportWidget->getCameraPos(),
                            highlightGizmoX,
                            highlightGizmoY,
                            highlightGizmoZ
                            ));

                //Also update the hitbox collision
                glm::vec4 transformedLocation =
                    (viewportWidget->getProjMatrix() * viewportWidget->getViewMatrix() * glm::vec4(gizmoPosition, 1.0f));
                float distanceFromCamera = transformedLocation.w;
                Transform t;

                //Y
                t.setPosition(gizmoPosition + glm::vec3(0.0f, 0.09f, 0.0f) * distanceFromCamera);
                t.setScale(0.1f * glm::vec3(0.4f, 2.2f, 0.4f) * distanceFromCamera);
                gizmoYPhysics->updateTransform(t);

                //X
                t.setPosition(gizmoPosition + glm::vec3(0.09f, 0.0f, 0.0f) * distanceFromCamera);
                t.setScale(0.1f * glm::vec3(2.2f, 0.4f, 0.4f) * distanceFromCamera);
                gizmoXPhysics->updateTransform(t);

                //Z
                t.setPosition(gizmoPosition + glm::vec3(0.0f, 0.0f, 0.09f) * distanceFromCamera);
                t.setScale(0.1f * glm::vec3(0.4f, 0.4f, 2.2f) * distanceFromCamera);
                gizmoZPhysics->updateTransform(t);
            } else {
                //Nothing's selected, so disable the gizmos
                if (isGizmoPhysicsInWorld) {
                    scene.getPhysicsManager()->removeRigidBody(gizmoYPhysics->getRigidBody());
                    scene.getPhysicsManager()->removeRigidBody(gizmoXPhysics->getRigidBody());
                    scene.getPhysicsManager()->removeRigidBody(gizmoZPhysics->getRigidBody());

                    isGizmoPhysicsInWorld = false;
                }
            }
        }

    }
}

