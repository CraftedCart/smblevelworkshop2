#version 330 core

in vec3 fragPos;

layout(location = 0) out vec4 color;

uniform vec4 tint;

void main() {
    color = tint;
}

