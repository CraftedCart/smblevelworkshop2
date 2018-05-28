#include "ws2editor/widget/ViewportWidget.hpp"
#include "ws2common/scene/MeshSceneNode.hpp"
#include "ws2common/MathUtils.hpp"
#include "ws2editor/project/ProjectManager.hpp"
#include "ws2editor/resource/ResourceManager.hpp"
#include "ws2editor/physics/PhysicsManager.hpp"
#include "ws2editor/PhysicsDebugDrawer.hpp"
#include "ws2editor/rendering/DebugRenderCommand.hpp"
#include "ws2editor/Config.hpp"
#include "ws2editor/task/ImportFileTask.hpp"
#include "ws2editor/WS2EditorInstance.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <BulletCollision/NarrowPhaseCollision/btRaycastCallback.h>
#include <Qt>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>
#include <QMimeData>
#include <QApplication>

namespace WS2Editor {
    namespace Widget {
        using namespace WS2Editor::Resource;
        using namespace WS2Editor::Rendering;
        using namespace WS2Common;
        using namespace WS2Common::Resource;

        ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent) {
            QTimer *updateTimer = new QTimer(this);
            connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
            updateTimer->start(1000.0f / 60.0f); //Cap the framerate at 60FPS TODO: Make this adjustable

            setFocusPolicy(Qt::StrongFocus); //Accept keyboard input
            setAcceptDrops(true); //Allow drag and drop onto this widget - used for file importing

            //Read all tips of the days
            QStringList tips;
            QFile tipFile(tr(":/Workshop2/Lang/tips-en_US.txt"));
            if (tipFile.open(QIODevice::ReadOnly)) {
                QTextStream in(&tipFile);

                while (!in.atEnd()) {
                    QString line = in.readLine();
                    tips.append(line);
                }

                tipFile.close();

                //Pick a random tip of the day
                tip = tips.at(WS2Common::MathUtils::randInt(0, tips.size() - 1));
            }

            emit postConstruct(*this);
        }

        ViewportWidget::~ViewportWidget() {
            makeCurrent(); //Need to have the context active to work with GL

            emit preDestroy(*this);

            renderManager->destroy();
            delete renderManager;
            delete tooltipPixmap;
            delete importPixmap;

            RenderManager::checkErrors("End of ~ViewportWidget");
        }

        qint64 ViewportWidget::getDeltaNanoseconds() {
            return deltaNanoseconds;
        }

        float ViewportWidget::getDeltaSeconds() {
            return deltaSeconds;
        }

        RenderManager* ViewportWidget::getRenderManager() {
            return renderManager;
        }

        void ViewportWidget::makeCurrentContext() {
            makeCurrent();
        }

        const glm::vec3 ViewportWidget::getCameraPos() {
            return cameraPos;
        }

        const glm::vec3 ViewportWidget::getTargetCameraPos() {
            return targetCameraPos;
        }

        const glm::mat4 ViewportWidget::getViewMatrix() {
            return view;
        }

        const glm::mat4 ViewportWidget::getProjMatrix() {
            return proj;
        }

        glm::vec3 ViewportWidget::zPlaneRaycast(const glm::vec3 viewportPos) {
            const glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width(), height());
            //(z + 1) / 2 as we need to convert NDC Z (-1 to 1) to a 0 - 1 scale
            const glm::vec3 mousePos = glm::vec3(viewportPos.x, height() - viewportPos.y, (viewportPos.z + 1.0f) / 2.0f);

            const glm::vec3 v = glm::unProject(mousePos, view, proj, viewport);
            PhysicsDebugDrawer *physicsDebugDrawer = Project::ProjectManager::getActiveProject()->getScene()->getPhysicsDebugDrawer();
            physicsDebugDrawer->drawLine(
                    MathUtils::toBtVector3(v + glm::vec3(-10.0f, 0.0f, 0.0f)),
                    MathUtils::toBtVector3(v + glm::vec3(10.0f, 0.0f, 0.0f)),
                    btVector3(1.0f, 1.0f, 0.0f)
                    );
            physicsDebugDrawer->drawLine(
                    MathUtils::toBtVector3(v + glm::vec3(0.0f, -10.0f, 0.0f)),
                    MathUtils::toBtVector3(v + glm::vec3(0.0f, 10.0f, 0.0f)),
                    btVector3(1.0f, 1.0f, 0.0f)
                    );
            physicsDebugDrawer->drawLine(
                    MathUtils::toBtVector3(v + glm::vec3(0.0f, 0.0f, -10.0f)),
                    MathUtils::toBtVector3(v + glm::vec3(0.0f, 0.0f, 10.0f)),
                    btVector3(1.0f, 1.0f, 0.0f)
                    );
            return v;
        }

        void ViewportWidget::initializeGL() {
            //Uses default format which should use OpenGL 3.3 core
            glewExperimental = GL_TRUE;
            glewInit();

            renderManager->init(width(), height());

            //Set the clear color
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            //Enable depth test and accept fragments closer to the camera
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            //Cull back faces
            glEnable(GL_CULL_FACE);

            //Load pixmaps
            tooltipPixmap = new QPixmap(":/Workshop2/Images/tooltip.png");
            importPixmap = new QPixmap(":/Workshop2/Icons/Import.png");

            //Mark the initial project as loaded, so that models added to the scene are also loaded
            Project::ProjectManager::getActiveProject()->getScene()->load();

            //TODO: Make the debug drawer an option, and if enabled, load for each scene
            Project::ProjectManager::getActiveProject()->getScene()->initPhysicsDebugDrawer();

            emit postInitializeGl(*this);

            //Start the elapsed timer
            elapsedTimer.start();
        }

        void ViewportWidget::resizeGL(int w, int h) {
            renderManager->resizeViewport(w, h);
        }

        /**
         * @note This disables GL_DEPTH_TEST because of the usage of QPainter
         * @todo This feels hacky as it's calling glm::project twice
         */
        void ViewportWidget::drawText(QPainter &painter, const glm::vec3 &pos, const QString &str, const QColor &col) {
            //TODO Stop calling glm::project twice
            //Projection matrix
            //glm::mat4 proj = glm::perspective(glm::radians(Config::cameraFov), (float) width() / (float) height(), 0.1f, 2000.0f);

            ////Camera matrix
            //glm::mat4 view = glm::lookAt(
            //*targetCameraPos,
            //*targetCameraPos + forward,
            //up
            //);

            glm::mat4 view2 = glm::lookAt(
                    targetCameraPos,
                    targetCameraPos + forward,
                    -up
                    );

            glm::vec4 viewport(0.0f, 0.0f, width(), height());
            glm::vec3 projected = glm::project(pos, view, proj, viewport);
            glm::vec3 projected2 = glm::project(pos, view2, proj, viewport);

            if (projected.z < 1) {
                painter.setPen(col);
                painter.setFont(QFont());
                painter.drawText(projected.x, projected2.y, str);
            }
        }

        bool ViewportWidget::isKeyDown(int key) {
            return keysDown.contains(key);
        }

        void ViewportWidget::preDraw() {
            QPoint cursorPos = QCursor::pos();
            QPoint tlWidgetPos = mapToGlobal(pos()); //tl = Top Left
            QPoint brWidgetPos = mapToGlobal(geometry().bottomRight()); //br = Bottom Right
            QPoint widgetCenter = (tlWidgetPos + brWidgetPos) / 2;

            if (mouseLocked) {
                QVector2D cursorDiffQt = QVector2D(widgetCenter - cursorPos);
                glm::vec2 cursorDiff = WS2Common::MathUtils::toGlmVec2(cursorDiffQt);

                //Hide cursor
                setCursor(Qt::BlankCursor);

                if (cameraNavMode == EnumCameraNav::NAV_FIRST_PERSON_FLY) {
                    cameraRot += cursorDiff * Config::cameraRotSpeed;
                    //Prevent camera from going whacko or upside down
                    cameraRot.y = glm::clamp(cameraRot.y, -glm::half_pi<float>(), glm::half_pi<float>());
                } else if (cameraNavMode == EnumCameraNav::NAV_ORBIT) {
                    //Calculate pivot center
                    glm::vec3 orbitForward = calcForwardVector(cameraRot);
                    glm::vec3 pivotCenter = targetCameraPos + (orbitForward * cameraPivotDistance);
                    //End calculate pivot center

                    //Rotate camera and calculate new camera position
                    cameraRot += cursorDiff * Config::cameraRotSpeed;
                    //Prevent camera from going whacko or upside down
                    cameraRot.y = glm::clamp(cameraRot.y, -glm::half_pi<float>(), glm::half_pi<float>());

                    orbitForward = calcForwardVector(cameraRot);
                    //Set both the target camera pos and the actual pos so weirdness doesn't happen
                    targetCameraPos = pivotCenter + (orbitForward * -cameraPivotDistance);
                    cameraPos = pivotCenter + (orbitForward * -cameraPivotDistance);
                }

                if (cameraNavMode == EnumCameraNav::NAV_FIRST_PERSON_FLY || cameraNavMode == EnumCameraNav::NAV_ORBIT) {
                    //Camera pos
                    float locPosSpeed = Config::cameraPosSpeed;
                    //TODO: Rebindable keys
                    if (isKeyDown(Qt::Key_Shift)) locPosSpeed *= Config::cameraPosSpeedUpMultiplier;
                    if (isKeyDown(Qt::Key_Alt)) locPosSpeed *= Config::cameraPosSlowDownMultiplier;
                    if (isKeyDown(Qt::Key_W)) targetCameraPos += forward * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_S)) targetCameraPos -= forward * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_D)) targetCameraPos += right * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_A)) targetCameraPos -= right * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_E)) targetCameraPos += up * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_Q)) targetCameraPos -= up * deltaSeconds * locPosSpeed;
                }
            } else {
                //Mouse not locked
                //Show cursor
                setCursor(Qt::CrossCursor);
            }

            switch (cameraNavMode) {
                case NAV_FIXED:
                    mouseLocked = false;
                    break;
                case NAV_FIRST_PERSON_FLY:
                case NAV_ORBIT:
                    mouseLocked = true;
                    break;
            }

            if (mouseLocked) {
                QCursor::setPos(widgetCenter);
            }

            //Camera inertia
            cameraPos = glm::mix(cameraPos, targetCameraPos, qBound(0.0f, Config::cameraInertia * deltaSeconds, 1.0f));

            calcVectors();

            //Update bullet physics
            btDynamicsWorld *dynamicsWorld = Project::ProjectManager::getActiveProject()->getScene()->getPhysicsManager()->getDynamicsWorld();
            dynamicsWorld->updateAabbs();

            emit postPreDraw(*this);
        }

        glm::vec3 ViewportWidget::calcForwardVector(glm::vec2 &rot) {
            return glm::vec3(
                    glm::cos(rot.y) * glm::sin(rot.x),
                    glm::sin(rot.y),
                    glm::cos(rot.y) * glm::cos(rot.x)
                    );
        }

        void ViewportWidget::calcVectors() {
            //Forward vector
            forward = calcForwardVector(cameraRot);

            //Right vector
            right = glm::vec3(
                    glm::sin(cameraRot.x - glm::half_pi<float>()),
                    0,
                    glm::cos(cameraRot.x - glm::half_pi<float>())
                    );

            //Up vector - Perpendicular to forward and right vector, so use cross product
            up = glm::cross(right, forward);
        }

        void ViewportWidget::paintGL() {
            preDraw();

            //Calculate delta time
            qint64 elapsedNanoseconds = elapsedTimer.nsecsElapsed();
            deltaNanoseconds = elapsedNanoseconds - prevNanosecondsElapsed;
            deltaSeconds = deltaNanoseconds / 1000000000.0f;
            prevNanosecondsElapsed = elapsedNanoseconds;

            //Need to enable this every frame as this gets disabled before drawing with QPainter
            glEnable(GL_CULL_FACE);

            //Projection matrix
            proj = glm::perspective(glm::radians(Config::cameraFov), (float) width() / (float) height(), Config::cameraNear, Config::cameraFar);

            //Camera matrix
            view = glm::lookAt(
                    cameraPos,
                    cameraPos + forward,
                    up
                    );

            //Model matrix (Identity matrix - Model will be at origin)
            glm::mat4 model = glm::mat4(1.0f);

            //Normal matrix
            glm::mat3 norm(glm::transpose(glm::inverse(model)));

            //The model-view-projection matrix (Matrix multiplication is the other way round)
            //glm::mat4 mvp = proj * view * model;

            glEnable(GL_DEPTH_TEST);
            glUseProgram(renderManager->progID);

            //Give the matrices to the bound shader
            //glUniformMatrix4fv(GLManager::shaderModelID, 1, GL_FALSE, &model[0][0]); //Model matrix set by recursiveDrawSceneNode
            glUniformMatrix4fv(renderManager->shaderViewID, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(renderManager->shaderProjID, 1, GL_FALSE, &proj[0][0]);
            glUniformMatrix3fv(renderManager->shaderNormID, 1, GL_FALSE, &norm[0][0]);

            ResourceScene *scene = Project::ProjectManager::getActiveProject()->getScene();
            if (scene != nullptr) {
                //Render the scene
                renderManager->enqueueRenderScene(
                        scene->getRootNode(),
                        Project::ProjectManager::getActiveProject()->getScene()
                        );

                emit postRenderScene(*scene);
            }

            //Physics debug drawing
            PhysicsDebugDrawer *physicsDebugDrawer = Project::ProjectManager::getActiveProject()->getScene()->getPhysicsDebugDrawer();
            if (Config::enablePhysicsDebugDrawing && physicsDebugDrawer != nullptr) {
                btDynamicsWorld *dynamicsWorld = Project::ProjectManager::getActiveProject()->getScene()->getPhysicsManager()->getDynamicsWorld();
                renderManager->enqueueRenderCommand(new DebugRenderCommand(renderManager, view, proj, dynamicsWorld, physicsDebugDrawer));
            }

            renderManager->renderQueue(defaultFramebufferObject());

            //Clear the debug drawer queue in case debug drawing is not enabled
            physicsDebugDrawer->clearBuffers();

            //Painter
            //Need to reset some OpenGL stuff before using QPainter is sane
            glDisable(GL_CULL_FACE);
            glUseProgram(0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            QPainter painter(this);
            this->painter = &painter;

            //Draw empty scene info (Like a tip of the day) if the scene is empty (The root node had only 1 child "Static" with 0 children)
            if (scene->getRootNode()->getChildCount() == 1 &&
                    scene->getRootNode()->getChildByIndex(0)->getChildCount() == 0) {
                drawEmptySceneInfo(painter);
            }

            //drawText(painter, glm::vec3(0.0f, 0.0f, 0.0f), QString("Origin"), QColor(255, 255, 255));
            glm::vec2 relativeCursorPos = getRelativeCursorPos();

            if (showDrop) {
                //Draw drag and drop info
                QFontMetrics fontMetrics = QFontMetrics(QFont());
                QString dropStr = tr("Drop to import files");

                painter.fillRect(relativeCursorPos.x, relativeCursorPos.y - importPixmap->height(),
                        importPixmap->width() + fontMetrics.width(dropStr) + 16, importPixmap->height(),
                        QBrush(QColor(0, 0, 0, 127)));

                painter.drawPixmap(relativeCursorPos.x, relativeCursorPos.y - importPixmap->height(), *importPixmap);

                painter.setPen(Qt::white);
                painter.drawText(WS2Common::MathUtils::toQPoint(
                            relativeCursorPos + glm::vec2(8 + importPixmap->width(), -10)),
                        dropStr);
            } else {
                drawObjectTooltipAtPos(painter, relativeCursorPos);
            }

            painter.end();

            //Check for errors
            RenderManager::checkErrors("End of ViewportWidget::paintGL()");

            //Emit the frameRendered Signal
            emit frameRendered(deltaNanoseconds);
        }

        void ViewportWidget::drawObjectTooltipAtPos(QPainter &painter, glm::vec2 pos) {
            btCollisionWorld::AllHitsRayResultCallback *rayCallback = ndcRaycast(
                    glm::vec2(
                        (pos.x / width() - 0.5f) * 2.0f,
                        -(pos.y / height() - 0.5f) * 2.0f
                        ),
                    cameraPos, Config::cameraFar, proj, view
                    );

            if (rayCallback->hasHit()) {
                bool wasEventHandled = false;
                emit onPhysicsObjectMouseOver(rayCallback, wasEventHandled);
                if (wasEventHandled) return;

                QVector<int> indices = sortAllHitsRayResultCallback(rayCallback);
                const btCollisionObject *closestObject = rayCallback->m_collisionObjects[indices.at(0)];

                WS2Common::Scene::SceneNode *node =
                    static_cast<WS2Common::Scene::SceneNode*>(closestObject->getUserPointer());

                painter.drawPixmap(pos.x, pos.y - tooltipPixmap->height(), *tooltipPixmap);

                painter.setPen(Qt::white);
                painter.drawText(WS2Common::MathUtils::toQPoint(pos + glm::vec2(20, -10)), node->getName());
            } else {
                emit onPhysicsObjectMouseOverNothing();
            }

            delete rayCallback;
        }

        void ViewportWidget::drawEmptySceneInfo(QPainter &painter) {
            painter.setPen(Qt::white);
            QFont f;

            f.setPixelSize(24);
            painter.setFont(f);
            painter.drawText(24, 48, tr("SMB Level Workshop 2"));
            painter.drawText(24, 120, tr("Tip of the day"));

            f.setPixelSize(14);
            painter.setFont(f);
            painter.drawText(24, 66, tr("Open a project or import models to get started"));
            painter.drawText(QRectF(24, 124, width() - 48, height() - 124 - 24), tip);

            //Plugin info
            WS2EditorInstance *ws2Instance = WS2EditorInstance::getInstance();
            painter.drawText(24, height() - 24, tr("%1 / %2 plugins initialized, %n plugin(s) failed to load", "", ws2Instance->getFailedPlugins().size())
                    .arg(ws2Instance->getInitializedPlugins().size())
                    .arg(ws2Instance->getLoadedPlugins().size())
                    );
        }

        void ViewportWidget::keyPressEvent(QKeyEvent *event) {
            keysDown.insert(event->key());
        }

        void ViewportWidget::keyReleaseEvent(QKeyEvent *event) {
            keysDown.remove(event->key());
        }

        void ViewportWidget::mousePressEvent(QMouseEvent *event) {
            //TODO: Rebindable mouse button
            switch (event->button()) {
                case Qt::LeftButton:
                    {
                        bool toggleSelect = QApplication::keyboardModifiers() & Qt::ControlModifier;
                        selectNodeAtScreenPos(getRelativeCursorPos(), toggleSelect);
                        break;
                    }
                case Qt::RightButton:
                    cameraNavMode = EnumCameraNav::NAV_FIRST_PERSON_FLY;
                    break;
                case Qt::MiddleButton:
                    cameraNavMode = EnumCameraNav::NAV_ORBIT;
                    break;
                default:
                    break;
            }

            emit onMousePressed(event);
        }

        void ViewportWidget::mouseReleaseEvent(QMouseEvent *event) {
            if (event->button() == Qt::RightButton || event->button() == Qt::MiddleButton) cameraNavMode = EnumCameraNav::NAV_FIXED;

            emit onMouseReleased(event);
        }

        void ViewportWidget::wheelEvent(QWheelEvent *event) {
            event->accept();

            //Zoom
            float dy = event->angleDelta().y() / 10.0f; //TODO:Make zoom around configurable
            targetCameraPos += forward * (float) dy;
            cameraPivotDistance -= dy;
            cameraPivotDistance = glm::max(cameraPivotDistance, 1.0f);
        }

        void ViewportWidget::dragEnterEvent(QDragEnterEvent *event) {
            if (event->mimeData()->hasUrls()) showDrop = true;
            event->acceptProposedAction();
        }

        void ViewportWidget::dragMoveEvent(QDragMoveEvent *event) {
            event->acceptProposedAction();
        }

        void ViewportWidget::dropEvent(QDropEvent *event) {
            //Import the dropped files
            const QMimeData *data = event->mimeData();
            if (data->hasUrls()) {
                QVector<Task::Task*> tasks;
                QList<QUrl> urls = data->urls();

                for (int i = 0; i < urls.size(); i++) {
                    QFile *f = new QFile(urls.at(i).toLocalFile());
                    tasks.append(new Task::ImportFileTask(f));
                }

                WS2EditorInstance::getInstance()->getTaskManager()->enqueueTasks(tasks);
            } else {
                qDebug() << "Invalid kind of data dropped onto the viewport";
            }

            showDrop = false;
            event->acceptProposedAction();
        }

        void ViewportWidget::dragLeaveEvent(QDragLeaveEvent *event) {
            showDrop = false;
            event->accept();
        }

        glm::vec2 ViewportWidget::getRelativeCursorPos() {
            QPoint cursorPos = QCursor::pos();
            QPoint tlWidgetPos = mapToGlobal(pos()); //tl = Top Left
            QVector2D relCursorPosQt = QVector2D(cursorPos - tlWidgetPos); //rel = Relative
            return WS2Common::MathUtils::toGlmVec2(relCursorPosQt);
        }

        btCollisionWorld::AllHitsRayResultCallback* ViewportWidget::ndcRaycast(const glm::vec2 pos, const glm::vec3 startPos,
                const float distance, const glm::mat4 proj, const glm::mat4 view) {
            //NDC = Normalized device coordinates
            glm::vec4 rayStartNdc(
                    pos.x,
                    pos.y,
                    -1.0f, //The near clipping plane maps to -1 Z in NDC
                    1.0f
                    );

            glm::vec4 rayEndNdc(
                    pos.x,
                    pos.y,
                    0.0f,
                    1.0f
                    );

            glm::mat4 inv = glm::inverse(proj * view);

            glm::vec4 rayStartWorld = inv * rayStartNdc;
            rayStartWorld /= rayStartWorld.w;

            glm::vec4 rayEndWorld = inv * rayEndNdc;
            rayEndWorld /= rayEndWorld.w;

            //Get the ray direction
            glm::vec3 rayDirWorld(rayEndWorld - rayStartWorld);
            rayDirWorld = glm::normalize(rayDirWorld);

            //Raycast
            glm::vec3 raycastEnd = startPos + rayDirWorld * distance;

            btCollisionWorld::AllHitsRayResultCallback *rayCallback = new btCollisionWorld::AllHitsRayResultCallback(
                    btVector3(WS2Common::MathUtils::toBtVector3(startPos)),
                    btVector3(WS2Common::MathUtils::toBtVector3(raycastEnd))
                    );
            rayCallback->m_flags = btTriangleRaycastCallback::kF_FilterBackfaces; //Ignore back faces

            Project::ProjectManager::getActiveProject()->getScene()->getPhysicsManager()->getDynamicsWorld()->rayTest(
                    btVector3(WS2Common::MathUtils::toBtVector3(startPos)),
                    btVector3(WS2Common::MathUtils::toBtVector3(raycastEnd)),
                    *rayCallback
                    );


            if (rayCallback->hasHit()) {
                for (int i = 0; i < rayCallback->m_collisionObjects.size(); i++) {
                    btVector3 hitPoint = rayCallback->m_hitPointWorld[i];
                    btVector3 hitNorm = rayCallback->m_hitNormalWorld[i];

                    Project::ProjectManager::getActiveProject()->getScene()->getPhysicsDebugDrawer()->drawLine(
                            hitPoint,
                            hitPoint + hitNorm * 10.0f,
                            btVector3(1.0f, 0.0f, 1.0f)
                            );
                }
            }

            return rayCallback;
        }

        QVector<int> ViewportWidget::sortAllHitsRayResultCallback(const btCollisionWorld::AllHitsRayResultCallback *rayCallback) {
            //Create an indices array and fill it incrementally(0, 1, 2, 3, etc. for as many hit objects as there are)
            QVector<int> indices(rayCallback->m_hitFractions.size());
            std::iota(indices.begin(), indices.end(), 0);

            //Sort the indices array based on the fractions
            class SortIndices {
                private:
                    const btScalar *arr;

                public:
                    SortIndices(const btScalar *arr) : arr(arr) {}
                    bool operator()(int a, int b) const { return arr[a] < arr[b]; }
            };
            std::sort(indices.begin(), indices.end(), SortIndices(&rayCallback->m_hitFractions[0]));

            return indices;
        }

        void ViewportWidget::selectNodeAtScreenPos(const glm::vec2 pos, bool toggleSelect) {
            btCollisionWorld::AllHitsRayResultCallback *rayCallback = ndcRaycast(
                    glm::vec2(
                        (pos.x / width() - 0.5f) * 2.0f,
                        -(pos.y / height() - 0.5f) * 2.0f
                        ),
                    cameraPos, Config::cameraFar, proj, view
                    );

            if (rayCallback->hasHit()) {
                bool wasEventHandled = false;
                emit onPhysicsObjectSelected(rayCallback, wasEventHandled);
                if (wasEventHandled) return;

                //Fetch the closest node
                QVector<int> indices = sortAllHitsRayResultCallback(rayCallback);

                //Select the hit node
                WS2Common::Scene::SceneNode *node =
                    static_cast<WS2Common::Scene::SceneNode*>(rayCallback->m_collisionObjects[indices.at(0)]->getUserPointer());

                if (toggleSelect) {
                    Project::ProjectManager::getActiveProject()->getScene()->getSelectionManager()->toggleSelect(node);
                } else {
                    Project::ProjectManager::getActiveProject()->getScene()->getSelectionManager()->selectOnly(node);
                }
            } else {
                //Deselect all if nothing was hit
                if (!toggleSelect) Project::ProjectManager::getActiveProject()->getScene()->getSelectionManager()->clearSelection();
            }

            delete rayCallback;
        }

    }
}

