#include "vec.h"

Vec2f vec2f(float x, float y) {
    return (Vec2f) {
        .x = x,
        .y = y
    };
}

Vec2f vec2fs(float x) {
    return vec2f(x, x);
}

Vec2f vec2fAdd(Vec2f a, Vec2f b) {
    return vec2f(a.x+b.x, a.y+b.y);
}

Vec2f vec2fSub(Vec2f a, Vec2f b) {
    return vec2f(a.x-b.x, a.y-b.y);
}

Vec2f vec2fMul(Vec2f a, Vec2f b) {
    return vec2f(a.x*b.x, a.y*b.y);
}

Vec2f vec2fMul3(Vec2f a, Vec2f b, Vec2f c) {
    return vec2fMul(vec2fMul(a, b), c);
}

Vec2f vec2fDiv(Vec2f a, Vec2f b) {
    return vec2f(a.x/b.x, a.y/b.y);
}

Vec2i vec2i(int x, int y) {
    return (Vec2i) {
        .x = x,
        .y = y
    };
}

Vec2i vec2is(int x) {
    return vec2i(x, x);
}

Vec2i vec2iAdd(Vec2i a, Vec2i b) {
    return vec2i(a.x+b.x, a.y+b.y);
}

Vec2i vec2iSub(Vec2i a, Vec2i b) {
    return vec2i(a.x-b.x, a.y-b.y);
}

Vec2i vec2iMul(Vec2i a, Vec2i b) {
    return vec2i(a.x*b.x, a.y*b.y);
}

Vec2i vec2iMul3(Vec2i a, Vec2i b, Vec2i c) {
    return vec2iMul(vec2iMul(a, b), c);
}

Vec2i vec2iDiv(Vec2i a, Vec2i b) {
    return vec2i(a.x/b.x, a.y/b.y);
}

Vec4f vec4f(float x, float y, float z, float w) {
    return (Vec4f) {
        .x = x,
        .y = y,
        .z = z,
        .w = w
    };
}

Vec4f vec4fs(float x) {
    return vec4f(x, x, x, x);
}

Vec4f vec4fAdd(Vec4f a, Vec4f b) {
    return vec4f(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Vec4f vec4fSub(Vec4f a, Vec4f b) {
    return vec4f(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Vec4f vec4fMul(Vec4f a, Vec4f b) {
    return vec4f(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

Vec4f vec4fDiv(Vec4f a, Vec4f b) {
    return vec4f(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}