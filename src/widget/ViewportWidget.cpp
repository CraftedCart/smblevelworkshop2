#include "widget/ViewportWidget.hpp"
#include "GLManager.hpp"
#include "MathUtils.hpp"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <Qt>
#include <QTimer>
#include <QDebug>
#include <QCursor>
#include <QPoint>
#include <QMouseEvent>

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
        delete texID;
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

        glGenVertexArrays(1, &vertArrayID);
        glBindVertexArray(vertArrayID);

        //Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
        //A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
        static const GLfloat vertBufData[] = {
            -1.0f,-1.0f,-1.0f, //triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, //triangle 1 : end
            1.0f, 1.0f,-1.0f, //triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, //triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
        };

        //Two UV coordinates for each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
        static const GLfloat uvBufData[] = {
            0.000059f, 1.0f-0.000004f,
            0.000103f, 1.0f-0.336048f,
            0.335973f, 1.0f-0.335903f,
            1.000023f, 1.0f-0.000013f,
            0.667979f, 1.0f-0.335851f,
            0.999958f, 1.0f-0.336064f,
            0.667979f, 1.0f-0.335851f,
            0.336024f, 1.0f-0.671877f,
            0.667969f, 1.0f-0.671889f,
            1.000023f, 1.0f-0.000013f,
            0.668104f, 1.0f-0.000013f,
            0.667979f, 1.0f-0.335851f,
            0.000059f, 1.0f-0.000004f,
            0.335973f, 1.0f-0.335903f,
            0.336098f, 1.0f-0.000071f,
            0.667979f, 1.0f-0.335851f,
            0.335973f, 1.0f-0.335903f,
            0.336024f, 1.0f-0.671877f,
            1.000004f, 1.0f-0.671847f,
            0.999958f, 1.0f-0.336064f,
            0.667979f, 1.0f-0.335851f,
            0.668104f, 1.0f-0.000013f,
            0.335973f, 1.0f-0.335903f,
            0.667979f, 1.0f-0.335851f,
            0.335973f, 1.0f-0.335903f,
            0.668104f, 1.0f-0.000013f,
            0.336098f, 1.0f-0.000071f,
            0.000103f, 1.0f-0.336048f,
            0.000004f, 1.0f-0.671870f,
            0.336024f, 1.0f-0.671877f,
            0.000103f, 1.0f-0.336048f,
            0.336024f, 1.0f-0.671877f,
            0.335973f, 1.0f-0.335903f,
            0.667969f, 1.0f-0.671889f,
            1.000004f, 1.0f-0.671847f,
            0.667979f, 1.0f-0.335851f
        };


        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertBufData), vertBufData, GL_STATIC_DRAW);

        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uvBufData), uvBufData, GL_STATIC_DRAW);

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
        progID = GLManager::loadShaders(&vertFile, &fragFile);

        //Load texture
        QImage bmpFile(":/Workshop2/Images/uvtemplate.png");
        texID = GLManager::loadTexture(&bmpFile);

        //Get uniform IDs
        shaderMvpID = glGetUniformLocation(progID, "mvp");
        shaderTexID = glGetUniformLocation(progID, "texSampler");

        //Start the elapsed timer
        elapsedTimer.start();
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
            glm::vec2 cursorDiff = MathUtils::toGlmVec2(&cursorDiffQt);

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

        //The model-view-projection matrix (Matrix multiplication is the other way round)
        glm::mat4 mvp = proj * view * model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(progID);

        //Draw the magnificent cube

        //Textures
        glActiveTexture(GL_TEXTURE0);
        texID->bind();
        glUniform1i(shaderTexID, 0);

        //First attribute buffer: Verts
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        //Second attribute buffer: UV
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

        //Give the MVP matrix to the bound shader - Cube 1
        glUniformMatrix4fv(shaderMvpID, 1, GL_FALSE, &mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); //Draw it

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

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

