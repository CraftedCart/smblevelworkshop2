#include "GLManager.hpp"
#include <QDebug>
#include <QDataStream>

namespace WS2 {
    namespace GLManager {

        //Define the extern variables
        GLuint progID;
        GLuint shaderMvpID;
        GLuint shaderTexID;

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

            //Compile vert shader
            qDebug() << "Compiling vertex shader:" << vertFile->fileName();
            const char *cVertCode = vertCode.toUtf8().constData();
            glShaderSource(vertID, 1, &cVertCode, NULL);
            glCompileShader(vertID);

            //Check vert shader
            glGetShaderiv(vertID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
                std::vector<char> vertShaderErrMsg(infoLogLength + 1);
                glGetShaderInfoLog(vertID, infoLogLength, NULL, &vertShaderErrMsg[0]);
                qWarning() << &vertShaderErrMsg[0];

                //TODO: Handle error
            }

            //Compile frag shader
            qDebug() << "Compiling fragment shader:" << fragFile->fileName();
            const char *cFragCode = fragCode.toUtf8().constData();
            glShaderSource(fragID, 1, &cFragCode, NULL);
            glCompileShader(fragID);

            //Check frag shader
            glGetShaderiv(fragID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
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
            glGetShaderiv(programID, GL_LINK_STATUS, &result);
            glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) {
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
            tex->setMinificationFilter(QOpenGLTexture::Linear);
            tex->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
            tex->generateMipMaps();

            return tex;
        }

        void renderMesh(const Model::Mesh &mesh, const glm::mat4 &mvpMatrix) {
            //Set up textures
            for (unsigned int i = 0; i < mesh.getTextures().size(); i++) {
                glActiveTexture(GL_TEXTURE0 + i);
                mesh.getTextures().at(i).texture->bind();
            }
            static const int texIDs[32] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
            glUniform1iv(shaderTexID, MAX_SHADER_TEXTURES, texIDs);

            //Draw the mesh
            glBindVertexArray(mesh.getVao());
            glDrawElements(GL_TRIANGLES, mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    }
}

