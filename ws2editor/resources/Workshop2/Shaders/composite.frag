#version 330 core

in vec2 uv;

out vec4 color;

uniform sampler2D texSampler;
uniform sampler2D cameraNormalsTexSampler;

float threshold(in float thr1, in float thr2, in float val) {
    if (val < thr1) return 0.0f;
    if (val > thr2) return 1.0f;
    return val;
}

//Averaged pixel intensity from 3 color channels
float avgIntensity(in vec4 pix) {
    return (pix.r + pix.g + pix.b) / 3.0f;
}

vec4 getPixel(in vec2 coords, in float dx, in float dy) {
    return texture(cameraNormalsTexSampler, coords + vec2(dx, dy));
}

//Returns pixel color
float isEdge(in vec2 coords) {
    float dxtex = 1.0f / 512.0f /* image width */;
    float dytex = 1.0f / 512.0f /* image height */;
    float pix[9];
    int k = -1;
    float delta;

    //Read neighboring pixel intensities
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            k++;
            pix[k] = avgIntensity(getPixel(coords,float(i) * dxtex, float(j) * dytex));
        }
    }

    //Average color differences around neighboring pixels
    delta = (abs(pix[1] - pix[7]) +
            abs(pix[5] - pix[3]) +
            abs(pix[0] - pix[8]) +
            abs(pix[2] - pix[6])
            ) / 4.0f;

    return threshold(0.25f, 0.4f, clamp(1.8f * delta, 0.0f, 1.0f));
}

void main() {
    float edge = isEdge(uv);
    if (edge > 0.5f) {
        color = edge * vec4(0.13f, 0.59f, 0.95f, 1.0f);
    } else {
        color = texture(texSampler, uv);
    }

    vec4 cameraNormal = texture(cameraNormalsTexSampler, uv);

    if (cameraNormal.x + cameraNormal.y + cameraNormal.z > 0.5f &&
            int(gl_FragCoord.x) % 4 == 0 &&
            int(gl_FragCoord.y) % 4 == 0) {
        color = vec4(0.13f, 0.59f, 0.95f, 1.0f);
    }
}
