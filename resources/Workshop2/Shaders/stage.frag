#version 330 core
in vec2 uv;
out vec3 color;

uniform sampler2D texSampler;

void main() {
    //Output color = col of texture at UV
    color = texture(texSampler, uv).rgb;
}

