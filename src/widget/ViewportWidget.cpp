#include "widget/ViewportWidget.hpp"
#include "GLManager.hpp"
#include "MathUtils.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <Qt>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPoint>
#include <QMouseEvent>
#include <QPainter>

namespace WS2 {
    ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent) {
        QTimer *updateTimer = new QTimer(this);
        connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
        updateTimer->start(1000.0f / 60.0f); //Cap the framerate at 60FPS TODO: Make this adjustable

        setFocusPolicy(Qt::StrongFocus); //Accept keyboard input
    }

    ViewportWidget::~ViewportWidget() {
        //Cleanup
        delete keysDown;
        delete cameraPos;
        delete cameraRot;

        makeCurrent();
        delete scene;
    }

    qint64 ViewportWidget::getDeltaNanoseconds() {
        return deltaNanoseconds;
    }

    float ViewportWidget::getDeltaSeconds() {
        return deltaSeconds;
    }

    void ViewportWidget::initializeGL() {
        //Uses default format which should use OpenGL 3.3 core
        glewExperimental = GL_TRUE;
        glewInit();

        //TODO: Load models from file picker
        //QFile f("<File path goes here>");
        //scene = new Model::Scene(f);

        //Set the clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //Enable depth test and accept fragments closer to the camera
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //Cull back faces
        glEnable(GL_CULL_FACE);

        //Load the shader
        QFile vertFile(":/Workshop2/Shaders/stage.vert");
        QFile fragFile(":/Workshop2/Shaders/stage.frag");
        GLManager::progID = GLManager::loadShaders(&vertFile, &fragFile);

        //Load texture
        //QImage bmpFile(":/Workshop2/Images/uvtemplate.png");
        //texID = GLManager::loadTexture(&bmpFile);

        //Get uniform IDs
        GLManager::shaderModelID = glGetUniformLocation(GLManager::progID, "modelMat");
        GLManager::shaderViewID = glGetUniformLocation(GLManager::progID, "viewMat");
        GLManager::shaderProjID = glGetUniformLocation(GLManager::progID, "projMat");
        GLManager::shaderNormID = glGetUniformLocation(GLManager::progID, "normMat");
        GLManager::shaderTexID = glGetUniformLocation(GLManager::progID, "texSampler");

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
        glm::mat4 proj = glm::perspective(glm::radians(fov), (float) width() / (float) height(), 0.1f, 1000.0f);

        //Camera matrix
        glm::mat4 view = glm::lookAt(
                *cameraPos,
                *cameraPos + forward,
                up
                );

        glm::mat4 view2 = glm::lookAt(
                *cameraPos,
                *cameraPos + forward,
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

            if (cameraNavMode == EnumCameraNav::NAV_FIRST_PERSON_FLY) {
                *cameraRot += cursorDiff * rotSpeed;
                //Prevent camera from going whacko or upside down
                cameraRot->y = glm::clamp(cameraRot->y, -glm::half_pi<float>(), glm::half_pi<float>());
            } else if (cameraNavMode == EnumCameraNav::NAV_ORBIT) {
                //Calculate pivot center
                glm::vec3 orbitForward = calcForwardVector(*cameraRot);
                glm::vec3 pivotCenter = *cameraPos + (orbitForward * cameraPivotDistance);
                //End calculate pivot center

                //Rotate camera and calculate new camera position
                *cameraRot += cursorDiff * rotSpeed;
                //Prevent camera from going whacko or upside down
                cameraRot->y = glm::clamp(cameraRot->y, -glm::half_pi<float>(), glm::half_pi<float>());

                orbitForward = calcForwardVector(*cameraRot);
                *cameraPos = pivotCenter + (orbitForward * -cameraPivotDistance);
            }

            if (cameraNavMode == EnumCameraNav::NAV_FIRST_PERSON_FLY || cameraNavMode == EnumCameraNav::NAV_ORBIT) {
                //Camera pos
                //TODO: Rebindable keys
                if (isKeyDown(Qt::Key_W)) *cameraPos += forward * deltaSeconds * posSpeed;
                if (isKeyDown(Qt::Key_S)) *cameraPos -= forward * deltaSeconds * posSpeed;
                if (isKeyDown(Qt::Key_D)) *cameraPos += right * deltaSeconds * posSpeed;
                if (isKeyDown(Qt::Key_A)) *cameraPos -= right * deltaSeconds * posSpeed;
                if (isKeyDown(Qt::Key_E)) *cameraPos += up * deltaSeconds * posSpeed;
                if (isKeyDown(Qt::Key_Q)) *cameraPos -= up * deltaSeconds * posSpeed;
            }
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
        glm::mat4 proj = glm::perspective(glm::radians(fov), (float) width() / (float) height(), 0.1f, 1000.0f);

        //Camera matrix
        glm::mat4 view = glm::lookAt(
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
        glUniformMatrix4fv(GLManager::shaderModelID, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(GLManager::shaderViewID, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(GLManager::shaderProjID, 1, GL_FALSE, &proj[0][0]);
        glUniformMatrix3fv(GLManager::shaderNormID, 1, GL_FALSE, &norm[0][0]);

        if (scene != nullptr) {
            QVector<Model::Mesh> &meshes = scene->getMeshes();
            for (int i = 0; i < meshes.size(); i++) {
                GLManager::renderMesh(meshes.at(i));
            }
        }

        //drawText(glm::vec3(0.0f, 0.0f, 0.0f), QString("Origin"), QColor(255, 255, 255));

        //Check for errors
        checkGLErrors("End of ViewportWidget::paintGL()");

        //Emit the frameRendered Signal
        emit frameRendered(deltaNanoseconds);
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

        //TODO: Relocate zooming code elsewhere
        float dy = event->angleDelta().y() / 30.0f;
        *cameraPos += forward * (float) dy;
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
}

