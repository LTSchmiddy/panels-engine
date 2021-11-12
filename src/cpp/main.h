#pragma once

#include <vector>

#include "ui/base/fragment.h"
#include "ui/core.h"


struct AppGlobal {
    int target_fps = 60;
    std::vector<UI::Fragment*> fragments;
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool show_demo_window = false;

    UI::FWindow* style_window;
    UI::FWindow* py_console_window;

};

extern AppGlobal g_AppGlobal;

void init_vlc();
