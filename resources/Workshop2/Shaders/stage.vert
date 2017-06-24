#version 330 core
//Location = 0 means the 0 in glEnableVertexAttribArray
layout(location = 0) in vec3 vertexPosition; //In model space
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec3 normal;
out vec2 uv;
out vec3 fragPos;

//Model, View, Projection, and Normal matrices
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat3 normMat;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(vertexPosition, 1);
    normal = normMat * vertexNormal;
    uv = vertexUV;
    fragPos = vec3(modelMat * vec4(vertexPosition, 1.0f));
}

