#ifndef SMBLEVELWORKSHOP2_GLMANAGER_HPP
#define SMBLEVELWORKSHOP2_GLMANAGER_HPP

#include "glplatform.hpp"
#include "resource/ResourceMesh.hpp"
#include <QFile>
#include <QString>
#include <QOpenGLTexture>
#include <QImage>

namespace WS2 {
    namespace GLManager {
        enum VertexAttribs {
            VERTEX_POSITION = 0,
            VERTEX_NORMAL = 1,
            VERTEX_TEX_COORD = 2
        };

        /**
         * @brief The maximum concurrently bound textures a shader can handle
         */
        const unsigned int MAX_SHADER_TEXTURES = 32;

        extern GLuint progID;
        extern GLuint shaderModelID;
        extern GLuint shaderViewID;
        extern GLuint shaderProjID;
        extern GLuint shaderNormID;
        extern GLuint shaderTexID;

        extern GLuint physicsDebugProgID;
        extern GLuint physicsDebugShaderModelID;
        extern GLuint physicsDebugShaderViewID;
        extern GLuint physicsDebugShaderProjID;

        /**
         * @brief Loads GLSL shaders
         *
         * @param vertFile The vertex shader file
         * @param fragFile The fragment shader file
         *
         * @return The shader ID
         */
        GLuint loadShaders(QFile *vertFile, QFile *fragFile);

        /**
         * @brief Finds occurances of placeholder text in shader strings, and replaces them with a constant
         *
         * @param s The string to find and replace in
         */
        void substituteShaderConstants(QString *s);

        /**
         * @brief Loads a texture for GL
         *
         * @param image The image to load
         *
         * @return The texture
         */
        QOpenGLTexture* loadTexture(QImage *image);

        /**
         * @brief Renders a mesh
         *
         * This assumes the appropriate shader is already bound.
         *
         * @param mesh The mesh to render
         */
        void renderMesh(const Resource::ResourceMesh *mesh);
    }
}

#endif

