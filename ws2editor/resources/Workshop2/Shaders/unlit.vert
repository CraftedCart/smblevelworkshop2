#version 330 core

layout(location = 0) in vec3 vertexPosition; //In model space

out vec3 fragPos;

//Model, View, Projection, and Normal matrices
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(vertexPosition, 1.0f);
}

