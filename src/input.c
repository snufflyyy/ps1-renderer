#include "input.h"

#include <stdbool.h>

#include <SDL3/SDL.h>
#include <cglm/cglm.h>

void input_manager_update(InputManager* input_manager) {
    input_manager->scroll_wheel = 0.0f;
}

void input_manager_event(InputManager* input_manager, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION: {
            glm_vec2_copy((vec2) { event->motion.x, event->motion.y }, input_manager->mouse_position);
        } break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            switch (event->button.button) {
                case SDL_BUTTON_LEFT: { input_manager->left_mouse_button = true; } break;
                case SDL_BUTTON_MIDDLE: { input_manager->middle_mouse_button = true; } break;
                case SDL_BUTTON_RIGHT: { input_manager->right_mouse_button = true; } break;
            }
        } break;
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            switch (event->button.button) {
                case SDL_BUTTON_LEFT: { input_manager->left_mouse_button = false; } break;
                case SDL_BUTTON_MIDDLE: { input_manager->middle_mouse_button = false; } break;
                case SDL_BUTTON_RIGHT: { input_manager->right_mouse_button = false; } break;
            }
        } break;
        case SDL_EVENT_MOUSE_WHEEL: {
            input_manager->scroll_wheel = event->wheel.y;
        } break;
    }
}
