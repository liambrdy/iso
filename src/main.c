#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60
#define DELTA_TIME (1.0f / FPS)

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    (void) source;
    (void) id;
    (void) length;
    (void) userParam;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

#define SHEET_WIDTH 192
#define SHEET_HEIGHT 288
#define TILES_ROW 6
#define TILES_COL 9
#define TILE_WIDTH (SHEET_WIDTH / TILES_ROW)
#define TILE_HEIGHT (SHEET_HEIGHT / TILES_COL)
#define TILE_UV_WIDTH (1.0f / TILES_ROW)
#define TILE_UV_HEIGHT (1.0f / TILES_COL)

typedef struct {
    Vec2i location;
    Vec2i texture;
    float yOffset;
} Tile;

#define TILE_MAX (6*1024)

static Renderer renderer = {0};
static Vec2f cameraPos = {0};
static Vec2f cameraVel = {0};
static float cameraZoom = 1.0f;
static Tile tiles[TILE_MAX];
static size_t tileCount = 0;

static int cmpTile(const void *a, const void *b) {
    Tile ta = *(const Tile *)a;
    Tile tb = *(const Tile *)b;
    return ta.location.x + ta.location.y < tb.location.x + tb.location.y;
}

void renderTiles(Tile *t, size_t tCount) {
    for (size_t i = 0; i < tCount; i++) {
        Tile tile = t[i];
        Vec2f screenPos = vec2f(0.5f * tile.location.x * TILE_WIDTH - 0.5f * tile.location.y * TILE_WIDTH, 0.25f * tile.location.x * TILE_HEIGHT + 0.25f * tile.location.y * TILE_HEIGHT + tile.yOffset);
        rendererTexturedRect(&renderer, screenPos, vec2f(TILE_WIDTH, TILE_HEIGHT), vec2f(tile.texture.x * TILE_UV_WIDTH, tile.texture.y * TILE_UV_HEIGHT), vec2f(TILE_UV_WIDTH, TILE_UV_HEIGHT), vec4fs(1.0f));
    }
}

void screenToTile(Vec2f screen) {
    
}

void addTile(Vec2i tile, Vec2i texture, float yOff) {
    assert(tileCount < TILE_MAX);
    Tile t = {
        .location = tile,
        .texture = texture,
        .yOffset = yOff
    };
    tiles[tileCount++] = t;
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Verlet", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        int major, minor;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &minor);
        printf("GL version %d.%d\n", major, minor);
    }

    if (SDL_GL_CreateContext(window) == NULL) {
        fprintf(stderr, "ERROR: Failed to create opengl context: %s\n", SDL_GetError());
        return 1;
    }

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "ERROR: Failed to initalize glew\n");
    }

    if (GLEW_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
    } else {
        fprintf(stderr, "WARNING! GLEW_ARB_debug_output is not available\n");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    rendererInit(&renderer);

    for (int y = -10; y < 10; y++) {
        for (int x = -10; x < 10; x++) {
            addTile(vec2i(x, y), vec2i(0, 8), 0.0f);
            addTile(vec2i(x, y), vec2i(1, 6), TILE_HEIGHT/2.0f);
        }
    }
    qsort(tiles, tileCount, sizeof(Tile), cmpTile);

    GLuint texture;
    {
        int w, h, c;
        stbi_set_flip_vertically_on_load(1);
        char *pixels = stbi_load("tiles.png", &w, &h, &c, STBI_rgb_alpha);
        if (pixels == NULL) {
            fprintf(stderr, "Failed to load texture file %s: %s\n", "tiles.png", stbi_failure_reason());
            return 1;
        }

        glActiveTexture(GL_TEXTURE0);
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        stbi_image_free(pixels);
    }

    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    float dt = DELTA_TIME;

    int w, h;

    cameraZoom = 3.0f;

    bool running = true;
    while (running) {
        const Uint32 start = SDL_GetTicks();
        SDL_Event event = {0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    running = false;
                } break;

                case SDL_KEYDOWN: {
                    const float UNITS = 100.0f;
                    switch (event.key.keysym.sym) {
                    }
                } break;

                case SDL_MOUSEWHEEL: {
                    cameraZoom += event.wheel.y * 0.5f;
                    cameraZoom = clampf(cameraZoom, 0.5f, 10.0f);
                } break;

                default: {}
            }
        }

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        const float SPEED = 100.0f;
        if (keystate[SDL_SCANCODE_A]) {
            cameraVel.x -= SPEED;
        } else if (keystate[SDL_SCANCODE_D]) {
            cameraVel.x += SPEED;
        }
        if (keystate[SDL_SCANCODE_W]) {
            cameraVel.y += SPEED;
        } else if (keystate[SDL_SCANCODE_S]) {
            cameraVel.y -= SPEED;
        }

        cameraPos = vec2fAdd(cameraPos, vec2fMul(cameraVel, vec2fs(DELTA_TIME)));
        cameraVel = vec2fs(0.0f);
        
        glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);

        for (size_t i = 0; i < tileCount; i++) {
            Tile *t = &tiles[i];
            t->yOffset += (sin(SDL_GetTicks() * 0.01f + t->location.y) + cos(SDL_GetTicks() * 0.01f + t->location.x));
        }

        rendererUse(&renderer);
        glUniform2f(renderer.uniforms[UNIFORM_SLOT_RESOLUTION], (float) w, (float) h);
        glUniform2f(renderer.uniforms[UNIFORM_SLOT_CAMERA_POS], cameraPos.x, cameraPos.y);
        glUniform1f(renderer.uniforms[UNIFORM_SLOT_CAMERA_ZOOM], cameraZoom);

        // rendererRect(&renderer, vec2fs(0.0f), vec2fs(300.0f), vec4fs(1.0f));
        renderTiles(tiles, tileCount);

        rendererFlush(&renderer);

        SDL_GL_SwapWindow(window);

        last = now;
        now = SDL_GetPerformanceCounter();
        float dt = (float)((now - last)*1000 / (float)SDL_GetPerformanceFrequency());

        const Uint32 duration = SDL_GetTicks() - start;
        const Uint32 deltaTime = 1000 / FPS;
        if (duration < deltaTime) {
            SDL_Delay(deltaTime - duration);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}