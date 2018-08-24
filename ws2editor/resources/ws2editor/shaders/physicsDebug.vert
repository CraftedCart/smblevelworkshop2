#version 330 core
//Location = 0 means the 0 in glEnableVertexAttribArray
layout(location = 0) in vec3 vertexPosition; //In model space
layout(location = 1) in vec3 vertexColor;

//Model, View, Projection matrices
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vertColor;

void main() {
    gl_Position = projMat * viewMat * vec4(vertexPosition, 1);
    vertColor = vertexColor;
}

