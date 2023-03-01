#version 330 core

layout (location = 0) out vec4 outColor;

in vec4 fragColor;

void main() {
    outColor = fragColor;
}