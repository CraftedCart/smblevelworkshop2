#include "GLManager.hpp"
#include <QDebug>
#include <QDataStream>

namespace WS2 {
    namespace GLManager {

        //Define the extern variables
        GLuint progID;
        GLuint shaderModelID;
        GLuint shaderViewID;
        GLuint shaderProjID;
        GLuint shaderNormID;
        GLuint shaderTexID;

        GLuint physicsDebugProgID;
        GLuint physicsDebugShaderModelID;
        GLuint physicsDebugShaderViewID;
        GLuint physicsDebugShaderProjID;

        /**
         * @brief Load, compile and link the shader files given
         *
         * @param vertFile The vertex shader file
         * @param fragFile The fragment shader file
         *
         * @return The program ID obtained when linking the shaders
         *
         * @todo Handle errors
         */
        GLuint loadShaders(QFile *vertFile, QFile *fragFile) {
            //Create the shaders
            GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);

            //Read the vert shader code
            if (!vertFile->open(QFile::ReadOnly)) {
                //Failed to open the file
                //TODO: Handle failing to open file
                qWarning() << "Failed to open file:" << vertFile->fileName();
                return 0;
            }
            QString vertCode = vertFile->readAll();
            vertFile->close();
            substituteShaderConstants(&vertCode);

            //Read the frag shader code
            if (!fragFile->open(QFile::ReadOnly)) {
                //Failed to open the file
                //TODO: Handle failing to open file
                qWarning() << "Failed to open file:" << fragFile->fileName();
                return 0;
            }
            QString fragCode = fragFile->readAll();
            fragFile->close();
            substituteShaderConstants(&fragCode);

            GLint result = GL_FALSE;
            int infoLogLength;

            QByteArray vertBytes = vertCode.toUtf8();
            const char *cVertCode = vertBytes.constData();
            QByteArray fragBytes = fragCode.toUtf8();
            const char *cFragCode = fragBytes.constData();

            //Compile vert shader
            qDebug() << "Compiling vertex shader:" << vertFile->fileName();
            glShaderSource(vertID, 1, &cVertCode, NULL);
            glCompileShader(vertID);

            //Check vert shader
            glGetShaderiv(vertID, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE) {
                glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &infoLogLength);
                std::vector<char> vertShaderErrMsg(infoLogLength + 1);
                glGetShaderInfoLog(vertID, infoLogLength, NULL, &vertShaderErrMsg[0]);
                qWarning() << &vertShaderErrMsg[0];

                //TODO: Handle error
            }

            //Compile frag shader
            qDebug() << "Compiling fragment shader:" << fragFile->fileName();
            glShaderSource(fragID, 1, &cFragCode, NULL);
            glCompileShader(fragID);

            //Check frag shader
            glGetShaderiv(fragID, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE) {
                glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &infoLogLength);
                std::vector<char> fragShaderErrMsg(infoLogLength + 1);
                glGetShaderInfoLog(fragID, infoLogLength, NULL, &fragShaderErrMsg[0]);
                qWarning() << &fragShaderErrMsg[0];

                //TODO: Handle error
            }

            //Link the program
            qDebug() << "Linking program";
            GLuint programID = glCreateProgram();
            glAttachShader(programID, vertID);
            glAttachShader(programID, fragID);
            glLinkProgram(programID);

            //Check program
            glGetProgramiv(programID, GL_LINK_STATUS, &result);
            if (result == GL_FALSE) {
                glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
                std::vector<char> progErrMsg(infoLogLength + 1);
                glGetProgramInfoLog(programID, infoLogLength, NULL, &progErrMsg[0]);
                qWarning() << &progErrMsg[0];

                //TODO: Handle error
            }

            //Cleanup
            glDetachShader(programID, vertID);
            glDetachShader(programID, fragID);

            glDeleteShader(vertID);
            glDeleteShader(fragID);

            return programID;
        }

        void substituteShaderConstants(QString *s) {
            s->replace("%MAX_SHADER_TEXTURES%", QString::number(MAX_SHADER_TEXTURES));
        }

        QOpenGLTexture* loadTexture(QImage *image) {
            QOpenGLTexture *tex = new QOpenGLTexture(image->mirrored());
            tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
            tex->setMagnificationFilter(QOpenGLTexture::Linear);
            tex->generateMipMaps();

            return tex;
        }

        void renderMesh(const Resource::ResourceMesh *mesh) {
            const QVector<Model::MeshSegment*>& segments = mesh->getMeshSegments();

            for (int i = 0; i < segments.size(); i++) {
                const Model::MeshSegment* segment = segments.at(i);

                //Set up textures
                for (int i = 0; i < segment->getTextures().size(); i++) {
                    glActiveTexture(GL_TEXTURE0 + i);
                    segment->getTextures().at(i)->getTexture()->bind();
                }
                static const int texIDs[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
                glUniform1iv(shaderTexID, MAX_SHADER_TEXTURES, texIDs);

                //Draw the segment
                glBindVertexArray(segment->getVao());
                glDrawElements(GL_TRIANGLES, segment->getIndices().size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }

    }
}

