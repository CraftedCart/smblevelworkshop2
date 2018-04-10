#version 330 core

in vec3 normal;
in vec4 cameraNormal;
in vec2 uv;
in vec3 fragPos;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 outCameraNormal;

uniform sampler2D texSampler;
uniform float texInfluence;
uniform vec4 tint;

void main() {
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 ambientLight = 0.3f * lightColor;
    vec3 norm = normalize(normal);

    vec3 lightPos = vec3(100000.0f, 500000.0f, 1000000.0f);
    vec3 lightDir = normalize(lightPos - fragPos); //Light direction
    float diff = max(dot(norm, lightDir), 0.0f);
    vec4 diffuse = diff * lightColor;

    vec4 textureColor = mix(vec4(1.0f), texture(texSampler, uv), texInfluence);

    color = (ambientLight + diffuse) * textureColor * tint;
    outCameraNormal = cameraNormal;
}

