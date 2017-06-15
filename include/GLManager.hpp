#ifndef SMBLEVELWORKSHOP2_GLMANAGER_HPP
#define SMBLEVELWORKSHOP2_GLMANAGER_HPP

#include "glplatform.hpp"
#include <QFile>
#include <QOpenGLTexture>
#include <QImage>

namespace WS2 {
    namespace GLManager {
        GLuint loadShaders(QFile *vertFile, QFile *fragFile);
        QOpenGLTexture* loadTexture(QImage *image);
    }
}

#endif

