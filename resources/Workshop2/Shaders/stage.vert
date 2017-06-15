#version 330 core
//Location = 0 means the 0 in glEnableVertexAttribArray
layout(location = 0) in vec3 vertexPosition_modelspace;
//Location = 1 means the 1 in glEnableVertexAttribArray
layout(location = 1) in vec2 vertexUV;

out vec2 uv;

//Model-View-Projection matrix
uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(vertexPosition_modelspace, 1);
    uv = vertexUV;
}

