#ifndef RENDERER_H_
#define RENDERER_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include "vec.h"

typedef enum {
    UNIFORM_SLOT_RESOLUTION = 0,
    UNIFORM_SLOT_CAMERA_POS = 1,
    UNIFORM_SLOT_CAMERA_ZOOM = 2,
    COUNT_UNIFORM_SLOT
} UniformSlot;

typedef enum {
    ATTR_POSITION = 0,
    ATTR_UV,
    ATTR_COLOR,
    COUNT_VERTEX_ATTR
} VertexAttr;

typedef struct {
    Vec2f pos;
    Vec2f uv;
    Vec4f color;
} Vertex;

#define RENDERER_VERTEX_BUFFER_CAP (3*600*1024)

typedef struct {
    GLuint vao;
    GLuint vbo;

    GLuint program;
    GLint uniforms[COUNT_UNIFORM_SLOT];

    Vertex vertices[RENDERER_VERTEX_BUFFER_CAP];
    size_t vertexCount;
} Renderer;

void rendererInit(Renderer *r);
void rendererUse(Renderer *r);
void rendererSync(Renderer *r);
void rendererDraw(Renderer *r);
void rendererFlush(Renderer *r);

void rendererVertex(Renderer *r, Vec2f p, Vec2f uv, Vec4f c);
void rendererTriangle(Renderer *r,
                      Vec2f p0, Vec2f p1, Vec2f p2,
                      Vec2f uv0, Vec2f uv1, Vec2f uv2,
                      Vec4f c0, Vec4f c1, Vec4f c2);
void rendererQuad(Renderer *r,
                  Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3,
                  Vec2f uv0, Vec2f uv1, Vec2f uv2, Vec2f uv3,
                  Vec4f c0, Vec4f c1, Vec4f c2, Vec4f c3);
void rendererRect(Renderer *r, Vec2f p, Vec2f size, Vec4f c);
void rendererTexturedRect(Renderer *r, Vec2f p, Vec2f size, Vec2f uv, Vec2f uvs, Vec4f c);
void rendererRectCentered(Renderer *r, Vec2f p, Vec2f size, Vec4f c);

#endif