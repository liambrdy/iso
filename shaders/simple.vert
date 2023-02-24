#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 color;

out vec2 fragPosition;
out vec2 fragUv;
out vec4 fragColor;

uniform vec2 resolution;
uniform vec2 cameraPos;
uniform float cameraZoom;

vec2 projectPoint(vec2 p) {
    return 2.0 * (p - cameraPos) * cameraZoom / resolution;
}

void main() {
    gl_Position = vec4(projectPoint(position), 0.0, 1.0);

    fragPosition = projectPoint(position) / 2.0;
    fragColor = color;
    fragUv = uv;
}