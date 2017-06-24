#version 330 core
//%MAX_SHADER_TEXTURES% will be replaced by the C++ code with an unsigned int
#define MAX_SHADER_TEXTURES %MAX_SHADER_TEXTURES%

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

out vec4 color;

uniform sampler2D texSampler[MAX_SHADER_TEXTURES];

void main() {
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 ambientLight = 0.3f * lightColor;
    vec3 lightPos = vec3(100000.0f, 100000.0f, 100000.0f);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos); //Light direction
    float diff = max(dot(norm, lightDir), 0.0f);
    vec4 diffuse = diff * lightColor;

    color = (ambientLight + diffuse) * texture(texSampler[0], uv);
}

