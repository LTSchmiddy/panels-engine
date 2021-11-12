#pragma once

#include <SDL2/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL2/SDL_opengles2.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include <box2d/box2d.h>
#include <tmx.h>


namespace Game {
class Engine {
    b2World* box_world;
    tmx_map* map;

public:
    Engine();
    ~Engine();

    void update();
    void onEvent(SDL_Event* event);

    void render();
};
}