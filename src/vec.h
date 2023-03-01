#ifndef VEC_H_
#define VEC_H_

typedef struct {
    float x, y;
} Vec2f;

Vec2f vec2f(float x, float y);
Vec2f vec2fs(float x);
Vec2f vec2fAdd(Vec2f a, Vec2f b);
Vec2f vec2fSub(Vec2f a, Vec2f b);
Vec2f vec2fMul(Vec2f a, Vec2f b);
Vec2f vec2fMul3(Vec2f a, Vec2f b, Vec2f c);
Vec2f vec2fDiv(Vec2f a, Vec2f b);

typedef struct {
    int x, y;
} Vec2i;

Vec2i vec2i(int x, int y);
Vec2i vec2is(int x);
Vec2i vec2iAdd(Vec2i a, Vec2i b);
Vec2i vec2iSub(Vec2i a, Vec2i b);
Vec2i vec2iMul(Vec2i a, Vec2i b);
Vec2i vec2iMul3(Vec2i a, Vec2i b, Vec2i c);
Vec2i vec2iDiv(Vec2i a, Vec2i b);

typedef struct {
    float x, y, z, w;
} Vec4f;

Vec4f vec4f(float x, float y, float z, float w);
Vec4f vec4fs(float x);
Vec4f vec4fAdd(Vec4f a, Vec4f b);
Vec4f vec4fSub(Vec4f a, Vec4f b);
Vec4f vec4fMul(Vec4f a, Vec4f b);
Vec4f vec4fDiv(Vec4f a, Vec4f b);

float lerpf(float a, float b, float t);
float clampf(float v, float min, float max);

#endif