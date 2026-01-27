#pragma once

#include <cglm/cglm.h>
#include <SDL3/SDL.h>

typedef struct InputManager {
    vec2 mouse_position;
    bool left_mouse_button;
    bool middle_mouse_button;
    bool right_mouse_button;

    float scroll_wheel;
} InputManager;

void input_manager_update(InputManager* input_manager);
void input_manager_event(InputManager* input_manager, SDL_Event* event);
