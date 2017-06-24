#version 330 core
//%MAX_SHADER_TEXTURES% will be replaced by the C++ code with an unsigned int
#define MAX_SHADER_TEXTURES %MAX_SHADER_TEXTURES%

in vec2 uv;

out vec4 color;

uniform sampler2D texSampler[MAX_SHADER_TEXTURES];

void main() {
    //Output color = col of texture at UV
    color = texture(texSampler[0], uv);
}

