#version 330 core

layout(location = 0) in vec3 vertexPosition; //In model space
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec3 normal;
out vec4 cameraNormal;
out vec2 uv;
out vec3 fragPos;

//Model, View, Projection, and Normal matrices
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat3 normMat;

uniform bool renderCameraNormals;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(vertexPosition, 1.0f);
    normal = normMat * vertexNormal;
    cameraNormal = vec4(inverse(transpose(viewMat * modelMat)) * vec4(normal, float(renderCameraNormals)));
    cameraNormal.xyz = cameraNormal.xyz * 0.5f + 0.5f;
    cameraNormal *= float(renderCameraNormals);
    uv = vertexUV;
    fragPos = vec3(modelMat * vec4(vertexPosition, 1.0f));
}

