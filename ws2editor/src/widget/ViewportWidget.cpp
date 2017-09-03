#include "widget/ViewportWidget.hpp"
#include "GLManager.hpp"
#include "MathUtils.hpp"
#include "project/ProjectManager.hpp"
#include "resource/ResourceManager.hpp"
#include "scene/MeshSceneNode.hpp"
#include "physics/PhysicsManger.hpp"
#include "PhysicsDebugDrawer.hpp"
#include "Config.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <Qt>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>

namespace WS2Editor {
    namespace Widget {
        ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent) {
            QTimer *updateTimer = new QTimer(this);
            connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
            updateTimer->start(1000.0f / 60.0f); //Cap the framerate at 60FPS TODO: Make this adjustable

            setFocusPolicy(Qt::StrongFocus); //Accept keyboard input

            //Read all tips of the days
            QStringList tips;
            QFile tipFile(tr(":/Workshop2/Lang/tips-en_US.txt"));
            if (tipFile.open(QIODevice::ReadOnly)) {
                QTextStream in(&tipFile);

                while (!in.atEnd()) {
                    QString line = in.readLine();
                    tips.append(line);
                }
            }

            //Pick a random tip of the day
            tip = tips.at(MathUtils::randInt(0, tips.size() - 1));
        }

        ViewportWidget::~ViewportWidget() {
            //Cleanup
            delete keysDown;
            delete cameraPos;
            delete targetCameraPos;
            delete cameraRot;

            makeCurrent();
        }

        qint64 ViewportWidget::getDeltaNanoseconds() {
            return deltaNanoseconds;
        }

        float ViewportWidget::getDeltaSeconds() {
            return deltaSeconds;
        }

        void ViewportWidget::makeCurrentContext() {
            makeCurrent();
        }

        void ViewportWidget::initializeGL() {
            //Uses default format which should use OpenGL 3.3 core
            glewExperimental = GL_TRUE;
            glewInit();

            //Set the clear color
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            //Enable depth test and accept fragments closer to the camera
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            //Cull back faces
            glEnable(GL_CULL_FACE);

            //Load the shaders
            QFile vertFile(":/Workshop2/Shaders/stage.vert");
            QFile fragFile(":/Workshop2/Shaders/stage.frag");
            GLManager::progID = GLManager::loadShaders(&vertFile, &fragFile);

            //Get uniform IDs
            GLManager::shaderModelID = glGetUniformLocation(GLManager::progID, "modelMat");
            GLManager::shaderViewID = glGetUniformLocation(GLManager::progID, "viewMat");
            GLManager::shaderProjID = glGetUniformLocation(GLManager::progID, "projMat");
            GLManager::shaderNormID = glGetUniformLocation(GLManager::progID, "normMat");
            GLManager::shaderTexID = glGetUniformLocation(GLManager::progID, "texSampler");

            //Load the physics debug shaders
            //TODO: Make loading this an option
            //QFile physVertFile(":/Workshop2/Shaders/physicsDebug.vert");
            //QFile physFragFile(":/Workshop2/Shaders/physicsDebug.frag");
            //GLManager::physicsDebugProgID = GLManager::loadShaders(&physVertFile, &physFragFile);

            //Get uniform IDs
            //GLManager::physicsDebugShaderViewID = glGetUniformLocation(GLManager::physicsDebugProgID, "viewMat");
            //GLManager::physicsDebugShaderProjID = glGetUniformLocation(GLManager::physicsDebugProgID, "projMat");

            //Mark the initial project as loaded, so that models added to the scene are also loaded
            Project::ProjectManager::getActiveProject()->getScene()->load();

            //TODO: Make the debug drawer an option, and if enabled, load for each scene
            //Project::ProjectManager::getActiveProject()->getScene()->initPhysicsDebugDrawer();

            //Start the elapsed timer
            elapsedTimer.start();
        }

        /**
         * @note This disables GL_DEPTH_TEST because of the usage of QPainter
         * @todo This feels hacky as it's calling glm::project twice
         */
        void ViewportWidget::drawText(const glm::vec3 &pos, const QString &str, const QColor &col) {
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
                    *targetCameraPos,
                    *targetCameraPos + forward,
                    -up
                    );

            glm::vec4 viewport(0.0f, 0.0f, width(), height());
            glm::vec3 projected = glm::project(pos, view, proj, viewport);
            glm::vec3 projected2 = glm::project(pos, view2, proj, viewport);

            if (projected.z < 1) {
                QPainter painter(this);
                painter.setPen(col);
                painter.setFont(QFont());
                painter.drawText(projected.x, projected2.y, str);
                painter.end();
            }
        }

        bool ViewportWidget::isKeyDown(int key) {
            return keysDown->contains(key);
        }

        void ViewportWidget::preDraw() {
            QPoint cursorPos = QCursor::pos();
            QPoint tlWidgetPos = mapToGlobal(pos()); //tl = Top Left
            QPoint brWidgetPos = mapToGlobal(geometry().bottomRight()); //br = Bottom Right
            QPoint widgetCenter = (tlWidgetPos + brWidgetPos) / 2;

            if (mouseLocked) {
                QVector2D cursorDiffQt = QVector2D(widgetCenter - cursorPos);
                glm::vec2 cursorDiff = MathUtils::toGlmVec2(cursorDiffQt);

                //Hide cursor
                setCursor(Qt::BlankCursor);

                if (cameraNavMode == EnumCameraNav::NAV_FIRST_PERSON_FLY) {
                    *cameraRot += cursorDiff * Config::cameraRotSpeed;
                    //Prevent camera from going whacko or upside down
                    cameraRot->y = glm::clamp(cameraRot->y, -glm::half_pi<float>(), glm::half_pi<float>());
                } else if (cameraNavMode == EnumCameraNav::NAV_ORBIT) {
                    //Calculate pivot center
                    glm::vec3 orbitForward = calcForwardVector(*cameraRot);
                    glm::vec3 pivotCenter = *targetCameraPos + (orbitForward * cameraPivotDistance);
                    //End calculate pivot center

                    //Rotate camera and calculate new camera position
                    *cameraRot += cursorDiff * Config::cameraRotSpeed;
                    //Prevent camera from going whacko or upside down
                    cameraRot->y = glm::clamp(cameraRot->y, -glm::half_pi<float>(), glm::half_pi<float>());

                    orbitForward = calcForwardVector(*cameraRot);
                    //Set both the target camera pos and the actual pos so weirdness doesn't happen
                    *targetCameraPos = pivotCenter + (orbitForward * -cameraPivotDistance);
                    *cameraPos = pivotCenter + (orbitForward * -cameraPivotDistance);
                }

                if (cameraNavMode == EnumCameraNav::NAV_FIRST_PERSON_FLY || cameraNavMode == EnumCameraNav::NAV_ORBIT) {
                    //Camera pos
                    float locPosSpeed = Config::cameraPosSpeed;
                    //TODO: Rebindable keys
                    if (isKeyDown(Qt::Key_Shift)) locPosSpeed *= Config::cameraPosSpeedUpMultiplier;
                    if (isKeyDown(Qt::Key_Alt)) locPosSpeed *= Config::cameraPosSlowDownMultiplier;
                    if (isKeyDown(Qt::Key_W)) *targetCameraPos += forward * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_S)) *targetCameraPos -= forward * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_D)) *targetCameraPos += right * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_A)) *targetCameraPos -= right * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_E)) *targetCameraPos += up * deltaSeconds * locPosSpeed;
                    if (isKeyDown(Qt::Key_Q)) *targetCameraPos -= up * deltaSeconds * locPosSpeed;
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
            *cameraPos = glm::mix(*cameraPos, *targetCameraPos, qBound(0.0f, Config::cameraInertia * deltaSeconds, 1.0f));

            calcVectors();
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
            forward = calcForwardVector(*cameraRot);

            //Right vector
            right = glm::vec3(
                    glm::sin(cameraRot->x - glm::half_pi<float>()),
                    0,
                    glm::cos(cameraRot->x - glm::half_pi<float>())
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

            //Projection matrix
            proj = glm::perspective(glm::radians(Config::cameraFov), (float) width() / (float) height(), Config::cameraNear, Config::cameraFar);

            //Camera matrix
            view = glm::lookAt(
                    *cameraPos,
                    *cameraPos + forward,
                    up
                    );

            //Model matrix (Identity matrix - Model will be at origin)
            glm::mat4 model = glm::mat4(1.0f);

            //Normal matrix
            glm::mat3 norm(glm::transpose(glm::inverse(model)));

            //The model-view-projection matrix (Matrix multiplication is the other way round)
            //glm::mat4 mvp = proj * view * model;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glUseProgram(GLManager::progID);

            //Give the matrices to the bound shader
            //glUniformMatrix4fv(GLManager::shaderModelID, 1, GL_FALSE, &model[0][0]); //Model matrix set by recursiveDrawSceneNode
            glUniformMatrix4fv(GLManager::shaderViewID, 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(GLManager::shaderProjID, 1, GL_FALSE, &proj[0][0]);
            glUniformMatrix3fv(GLManager::shaderNormID, 1, GL_FALSE, &norm[0][0]);

            Resource::ResourceScene *scene = Project::ProjectManager::getActiveProject()->getScene();

            if (scene != nullptr) {
                recursiveDrawSceneNode(scene->getRootNode(), glm::mat4(1.0f));
            }

            //Draw empty scene info (Like a tip of the day) if the scene is empty (The root node had only 1 child "Static" with 0 children)
            if (scene->getRootNode()->getChildCount() == 1 &&
                    scene->getRootNode()->getChildByIndex(0)->getChildCount() == 0) {
                drawEmptySceneInfo();
            }

            //drawText(glm::vec3(0.0f, 0.0f, 0.0f), QString("Origin"), QColor(255, 255, 255));

            //Physics debug drawing
            PhysicsDebugDrawer *physicsDebugDrawer = Project::ProjectManager::getActiveProject()->getScene()->getPhysicsDebugDrawer();
            if (physicsDebugDrawer != nullptr) {
                glUseProgram(GLManager::physicsDebugProgID);

                //Give the matrices to the bound shader
                glUniformMatrix4fv(GLManager::physicsDebugShaderModelID, 1, GL_FALSE, &model[0][0]);
                glUniformMatrix4fv(GLManager::physicsDebugShaderViewID, 1, GL_FALSE, &view[0][0]);
                glUniformMatrix4fv(GLManager::physicsDebugShaderProjID, 1, GL_FALSE, &proj[0][0]);

                Project::ProjectManager::getActiveProject()->getScene()->getPhysicsManager()->getDynamicsWorld()->debugDrawWorld();
                physicsDebugDrawer->drawAll();
            }

            //Check for errors
            checkGLErrors("End of ViewportWidget::paintGL()");

            //Emit the frameRendered Signal
            emit frameRendered(deltaNanoseconds);
        }

        void ViewportWidget::recursiveDrawSceneNode(WS2Common::Scene::SceneNode *node, const glm::mat4 parentTransform) const {
            glm::mat4 transform = parentTransform;
            transform = glm::translate(transform, node->getPosition());
            transform = glm::rotate(transform, node->getRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
            transform = glm::rotate(transform, node->getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
            transform = glm::rotate(transform, node->getRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::scale(transform, node->getScale());

            if (const Scene::MeshSceneNode *mesh = dynamic_cast<const Scene::MeshSceneNode*>(node)) {
                glUniformMatrix4fv(GLManager::shaderModelID, 1, GL_FALSE, &transform[0][0]);
                GLManager::renderMesh(mesh->getMesh());
            }

            for (int i = 0; i < node->getChildren().size(); i++) {
                recursiveDrawSceneNode(node->getChildren().at(i), transform);
            }
        }

        void ViewportWidget::drawEmptySceneInfo() {
            QPainter p(this);
            p.setPen(Qt::white);
            QFont f;

            f.setPixelSize(24);
            p.setFont(f);
            p.drawText(24, 48, tr("SMB Level Workshop 2"));
            p.drawText(24, 120, tr("Tip of the day"));

            f.setPixelSize(14);
            p.setFont(f);
            p.drawText(24, 66, tr("Open a project or import models to get started"));
            p.drawText(QRectF(24, 124, width() - 48, height() - 124 - 24), tip);

            p.end();
        }

        void ViewportWidget::keyPressEvent(QKeyEvent *event) {
            keysDown->insert(event->key());
        }

        void ViewportWidget::keyReleaseEvent(QKeyEvent *event) {
            keysDown->remove(event->key());
        }

        void ViewportWidget::mousePressEvent(QMouseEvent *event) {
            //TODO: Rebindable mouse button
            switch (event->button()) {
                case Qt::LeftButton:
                    {
                        QPoint cursorPos = QCursor::pos();
                        QPoint tlWidgetPos = mapToGlobal(pos()); //tl = Top Left
                        QVector2D relCursorPosQt = QVector2D(cursorPos - tlWidgetPos); //rel = Relative
                        glm::vec2 relCursorPos = MathUtils::toGlmVec2(relCursorPosQt);
                        selectNodeAtScreenPos(relCursorPos);
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
        }

        void ViewportWidget::mouseReleaseEvent(QMouseEvent *event) {
            if (event->button() == Qt::RightButton || event->button() == Qt::MiddleButton) cameraNavMode = EnumCameraNav::NAV_FIXED;
        }

        void ViewportWidget::wheelEvent(QWheelEvent *event) {
            event->accept();

            //Zoom
            float dy = event->angleDelta().y() / 10.0f; //TODO:Make zoom around configurable
            *targetCameraPos += forward * (float) dy;
            cameraPivotDistance -= dy;
            cameraPivotDistance = glm::max(cameraPivotDistance, 1.0f);
        }

        void ViewportWidget::checkGLErrors(QString location) {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                //Errors occured
                QString errString;

                switch(err) {
                    case GL_INVALID_OPERATION:
                        errString = "INVALID_OPERATION";
                        break;
                    case GL_INVALID_ENUM:
                        errString = "INVALID_ENUM";
                        break;
                    case GL_INVALID_VALUE:
                        errString = "INVALID_VALUE";
                        break;
                    case GL_OUT_OF_MEMORY:
                        errString = "OUT_OF_MEMORY";
                        break;
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                        errString = "INVALID_FRAMEBUFFER_OPERATION";
                        break;
                }

                qWarning() << "GL Error:" << err << "-" << errString << "- Found at:" << location;
            }
        }

        void ViewportWidget::selectNodeAtScreenPos(const glm::vec2 pos) {
            //NDC = Normalized device coordinates
            glm::vec4 rayStartNdc(
                    (pos.x / width() - 0.5f) * 2.0f,
                    -(pos.y / height() - 0.5f) * 2.0f,
                    -1.0f, //The near clipping plane maps to -1 Z in NDC
                    1.0f
                    );

            glm::vec4 rayEndNdc(
                    (pos.x / width() - 0.5f) * 2.0f,
                    -(pos.y / height() - 0.5f) * 2.0f,
                    0.0f,
                    1.0f
                    );

            glm::mat4 inv = glm::inverse(proj * view);

            glm::vec4 rayStartWorld = inv * rayStartNdc;
            rayStartWorld /= rayStartWorld.w;

            glm::vec4 rayEndWorld = inv * rayEndNdc;
            rayEndWorld /= rayEndWorld.w;

            //Get the ray direction
            glm::vec3 rayDirWorld(rayStartWorld - rayEndWorld);
            rayDirWorld = glm::normalize(rayDirWorld);

            //Raycast
            glm::vec3 raycastEnd = *cameraPos + rayDirWorld * 2000.0f;
            raycastEnd *= -1.0f; //I have no idea why I need to negate everything here, but I do, and it works

            btCollisionWorld::ClosestRayResultCallback rayCallback(
                    btVector3(MathUtils::toBtVector3(*cameraPos)),
                    btVector3(MathUtils::toBtVector3(raycastEnd))
                    );

            Project::ProjectManager::getActiveProject()->getScene()->getPhysicsManager()->getDynamicsWorld()->rayTest(
                    btVector3(MathUtils::toBtVector3(*cameraPos)),
                    btVector3(MathUtils::toBtVector3(raycastEnd)),
                    rayCallback
                    );

            if (rayCallback.hasHit()) {
                //Select the hit node
                WS2Common::Scene::SceneNode *node = static_cast<WS2Common::Scene::SceneNode*>(rayCallback.m_collisionObject->getUserPointer());
                Project::ProjectManager::getActiveProject()->getScene()->getSelectionManager()->selectOnly(node);
            } else {
                //Deselect all if nothing was hit
                Project::ProjectManager::getActiveProject()->getScene()->getSelectionManager()->clearSelection();
            }
        }
    }
}

