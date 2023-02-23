#version 330 core

layout (location = 0) out vec4 outColor;

in vec2 fragUv;
in vec4 fragColor;

uniform vec2 resolution;
uniform sampler2D tiles;

void main() {
    outColor = texture(tiles, fragUv);
    // outColor = vec4(fragUv, 0.0, 1.0);
}