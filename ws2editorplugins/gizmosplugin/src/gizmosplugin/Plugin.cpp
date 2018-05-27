#include "gizmosplugin/Plugin.hpp"
#include "gizmosplugin/RenderGlobals.hpp"
#include "gizmosplugin/rendering/TranslateGizmoRenderCommand.hpp"
#include "ws2editor/physics/PhysicsUtils.hpp"
#include "ws2common/model/ModelLoader.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <QAction>
#include <QMessageBox>
#include <QDebug>

namespace WS2EditorPlugins {
    namespace GizmosPlugin {
        using namespace WS2EditorPlugins::GizmosPlugin::Rendering;
        using namespace WS2Editor;
        using namespace WS2Editor::Widget;

        bool Plugin::init() {
            //Hook into the stage editor window by calling onStageEditorWindowConstructed after it's been created
            connect(WS2EditorInstance::getInstance(), &WS2EditorInstance::onStageEditorWindowConstructed,
                    this, &Plugin::onStageEditorWindowConstructed,
                    Qt::DirectConnection);

            //The plugin initialized successfully, return true
            qDebug() << "WS2Editor GizmosPlugin successfully initialized";
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
            connect(viewportWidget, &ViewportWidget::postRenderScene,
                    this, &Plugin::updateGizmos,
                    Qt::DirectConnection);
            connect(viewportWidget, &ViewportWidget::preDestroy,
                    this, &Plugin::onViewportWidgetPreDestroy,
                    Qt::DirectConnection);
        }

        void Plugin::onViewportWidgetPostConstruct(ViewportWidget &viewportWidget) {
            Q_UNUSED(viewportWidget);

            //TODO
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
            if (PhysicsUtils::doesAlignedObjectArrayContain(
                        rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoYPhysics->getRigidBody())) {
                qDebug() << "Gizmo transform Y NYI";
                outHandled = true;
            } else if (PhysicsUtils::doesAlignedObjectArrayContain(
                        rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoXPhysics->getRigidBody())) {
                qDebug() << "Gizmo transform X NYI";
                outHandled = true;
            } else if (PhysicsUtils::doesAlignedObjectArrayContain(
                        rayCallback->m_collisionObjects, (const btCollisionObject*) gizmoZPhysics->getRigidBody())) {
                qDebug() << "Gizmo transform Z NYI";
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
                    highlightGizmoY = false;
                    highlightGizmoX = false;
                    highlightGizmoZ = false;
                }
        }

        void Plugin::onViewportWidgetPhysicsObjectMouseOverNothing() {
            //Don't highlight any gizmos
            highlightGizmoY = false;
            highlightGizmoX = false;
            highlightGizmoZ = false;
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
                    aabb = initialMeshData->getMesh()->getAabb();
                    aabb.offsetBy(initialNode->getPosition());
                } else {
                    aabb = AABB3(initialNode->getPosition(), initialNode->getPosition());
                }

                for (SceneNode *node : selectionManager->getSelectedObjects()) {
                    MeshNodeData *meshData = scene.getMeshNodeData(node->getUuid());

                    if (meshData != nullptr) {
                        AABB3 newAabb = meshData->getMesh()->getAabb();
                        newAabb.offsetBy(node->getPosition());
                        aabb.mergeWith(newAabb);
                    } else {
                        aabb.mergeWith(node->getPosition());
                    }
                }

                //Get the middle point of the AABB, and draw the gizmo there
                const glm::vec3 center = aabb.getCenter();
                renderManager->enqueueRenderCommand(new TranslateGizmoRenderCommand(
                            renderManager,
                            glm::translate(center),
                            viewportWidget->getViewMatrix(),
                            viewportWidget->getProjMatrix(),
                            viewportWidget->getCameraPos(),
                            highlightGizmoX,
                            highlightGizmoY,
                            highlightGizmoZ
                            ));

                //Also update the hitbox collision
                float distanceFromCamera = glm::length(
                        viewportWidget->getProjMatrix() * viewportWidget->getViewMatrix() * glm::vec4(center, 1.0f));
                Transform t;

                //Y
                t.setPosition(center + glm::vec3(0.0f, 0.05f, 0.0f) * distanceFromCamera);
                t.setScale(0.1f * glm::vec3(0.2f, 1.2f, 0.2f) * distanceFromCamera);
                gizmoYPhysics->updateTransform(t);

                //X
                t.setPosition(center + glm::vec3(0.05f, 0.0f, 0.0f) * distanceFromCamera);
                t.setScale(0.1f * glm::vec3(1.2f, 0.2f, 0.2f) * distanceFromCamera);
                gizmoXPhysics->updateTransform(t);

                //Z
                t.setPosition(center + glm::vec3(0.0f, 0.0f, 0.05f) * distanceFromCamera);
                t.setScale(0.1f * glm::vec3(0.2f, 0.2f, 1.2f) * distanceFromCamera);
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

