#include "renderer.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

typedef struct {
    size_t offset;
    size_t comps;
} AttrDef;

static const AttrDef vertexAttrDefs[COUNT_VERTEX_ATTR] = {
    [ATTR_POSITION] = {
        .offset = offsetof(Vertex, pos),
        .comps = 2
    },
    [ATTR_UV] = {
        .offset = offsetof(Vertex, uv),
        .comps = 2,
    },
    [ATTR_COLOR] = {
        .offset = offsetof(Vertex, color),
        .comps = 4
    }
};
static_assert(COUNT_VERTEX_ATTR == 3, "More vertex attributes have been added");

typedef struct {
    UniformSlot slot;
    const char *name;
} UniformDef;

static const UniformDef uniformDefs[COUNT_UNIFORM_SLOT] = {
    [UNIFORM_SLOT_RESOLUTION] = {
        .slot = UNIFORM_SLOT_RESOLUTION,
        .name = "resolution"
    },
    [UNIFORM_SLOT_CAMERA_POS] = {
        .slot = UNIFORM_SLOT_CAMERA_POS,
        .name = "cameraPos"
    },
    [UNIFORM_SLOT_CAMERA_ZOOM] = {
        .slot = UNIFORM_SLOT_CAMERA_ZOOM,
        .name = "cameraZoom"
    }
};
static_assert(COUNT_UNIFORM_SLOT == 3, "More uniform slots have been added");

static const char *shaderTypeAsCStr(GLuint shader) {
    switch (shader) {
        case GL_VERTEX_SHADER:
            return "GL_VERTEX_SHADER";
        case GL_FRAGMENT_SHADER:
            return "GL_FRAGMENT_SHADER";
        default:
            return "(Unknown)";
    }
}

static bool compileShaderSource(const GLchar *source, GLenum shaderType, GLuint *shader) {
    *shader = glCreateShader(shaderType);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    GLint compiled = 0;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLchar message[1024];
        GLsizei messageSize = 0;
        glGetShaderInfoLog(*shader, sizeof(message), &messageSize, message);
        fprintf(stderr, "ERROR: could not compile %s\n", shaderTypeAsCStr(shaderType));
        fprintf(stderr, "%.*s\n", messageSize, message);
        return false;
    }

    return true;
}

static bool compileShaderFile(const char *filePath, GLenum shaderType, GLuint *shader) {
    bool result = true;

    char *source = readEntireFile(filePath);

    if (!compileShaderSource(source, shaderType, shader)) {
        fprintf(stderr, "ERROR: failed to compile `%s` shader file\n", filePath);
        free(source);
        return result;
    }

    free(source);
    return result;
}

static void attachShadersToProgram(GLuint *shaders, size_t shaderCount, GLuint program) {
    for (size_t i = 0; i < shaderCount; i++) {
        glAttachShader(program, shaders[i]);
    }
}

static bool linkProgram(GLuint program, const char *filePath, size_t line) {
    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLchar message[1024];
        GLsizei messageSize = 0;

        glGetProgramInfoLog(program, sizeof(message), &messageSize, message);
        fprintf(stderr, "%s:%zu: Program Linking: %.*s\n", filePath, line, messageSize, message);
    }

    return linked;
}

void rendererInit(Renderer *r) {
    glCreateVertexArrays(1, &r->vao);
    glBindVertexArray(r->vao);
    glCreateBuffers(1, &r->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*RENDERER_VERTEX_BUFFER_CAP, NULL, GL_DYNAMIC_DRAW);

    for (size_t i = 0; i < COUNT_VERTEX_ATTR; i++) {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, vertexAttrDefs[i].comps, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) vertexAttrDefs[i].offset);
    }

    GLuint shaders[2] = {0};
    if (!compileShaderFile("shaders/simple.vert", GL_VERTEX_SHADER, &shaders[0])) {
        exit(1);
    }

    if (!compileShaderFile("shaders/tile.frag", GL_FRAGMENT_SHADER, &shaders[1])) {
        exit(1);
    }

    r->program = glCreateProgram();
    attachShadersToProgram(shaders, sizeof(shaders)/sizeof(shaders[0]), r->program);
    if (!linkProgram(r->program, __FILE__, __LINE__)) {
        exit(1);
    }
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);

    for (int i = 0; i < COUNT_UNIFORM_SLOT; i++) {
        r->uniforms[i] = glGetUniformLocation(r->program, uniformDefs[i].name);
    }
}

void rendererUse(Renderer *r) {
    glBindVertexArray(r->vao);
    glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
    glUseProgram(r->program);
}

void rendererSync(Renderer *r) {
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*r->vertexCount, r->vertices);
}

void rendererDraw(Renderer *r) {
    glDrawArrays(GL_TRIANGLES, 0, r->vertexCount);
}

void rendererFlush(Renderer *r) {
    rendererSync(r);
    rendererDraw(r);
    r->vertexCount = 0;
}

void rendererVertex(Renderer *r, Vec2f p, Vec2f uv, Vec4f c) {
    if (r->vertexCount >= RENDERER_VERTEX_BUFFER_CAP) rendererFlush(r);
    Vertex v = {
        .pos = p,
        .uv = uv,
        .color = c
    };
    r->vertices[r->vertexCount++] = v;
}

void rendererTriangle(Renderer *r,
                      Vec2f p0, Vec2f p1, Vec2f p2,
                      Vec2f uv0, Vec2f uv1, Vec2f uv2,
                      Vec4f c0, Vec4f c1, Vec4f c2) {
    rendererVertex(r, p0, uv0, c0);
    rendererVertex(r, p1, uv1, c1);
    rendererVertex(r, p2, uv2, c2);
}

void rendererQuad(Renderer *r,
                  Vec2f p0, Vec2f p1, Vec2f p2, Vec2f p3,
                  Vec2f uv0, Vec2f uv1, Vec2f uv2, Vec2f uv3,
                  Vec4f c0, Vec4f c1, Vec4f c2, Vec4f c3) {
    rendererTriangle(r, p0, p1, p2, uv0, uv1, uv2, c0, c1, c2);
    rendererTriangle(r, p1, p2, p3, uv1, uv2, uv3, c1, c2, c3);
}

void rendererRect(Renderer *r, Vec2f p, Vec2f s, Vec4f c) {
    Vec2f uv = vec2fs(0.0);
    rendererQuad(
        r,
        p, vec2fAdd(p, vec2f(s.x, 0)), vec2fAdd(p, vec2f(0, s.y)), vec2fAdd(p, s),
        uv, vec2fAdd(uv, vec2f(1.0, 0)), vec2fAdd(uv, vec2f(0, 1.0)), vec2fAdd(uv, vec2fs(1.0)),
        c, c, c, c
    );
}

void rendererTexturedRect(Renderer *r, Vec2f p, Vec2f s, Vec2f uv, Vec2f uvs, Vec4f c) {
    rendererQuad(
        r,
        p, vec2fAdd(p, vec2f(s.x, 0)), vec2fAdd(p, vec2f(0, s.y)), vec2fAdd(p, s),
        uv, vec2fAdd(uv, vec2f(uvs.x, 0)), vec2fAdd(uv, vec2f(0, uvs.y)), vec2fAdd(uv, uvs),
        c, c, c, c
    );
}

void rendererRectCentered(Renderer *r, Vec2f p, Vec2f size, Vec4f c) {
    rendererRect(
        r,
        vec2fSub(p, vec2fDiv(size, vec2fs(2.0))),
        size, c
    );
}