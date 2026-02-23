#pragma once

#include <SDL3/SDL.h>

#include "camera.h"
#include "window.h"

typedef struct Player {
    vec3 position;
    Camera camera;

    float movement_speed;

    bool space_pressed;
    bool shift_pressed;
    bool w_pressed;
    bool a_pressed;
    bool s_pressed;
    bool d_pressed;
} Player;

Player player_create(vec3 position, Window* window);
void player_event(Player* player, SDL_Event* event);
void player_update(Player* player, double delta_time);
void player_destroy(Player* player);
