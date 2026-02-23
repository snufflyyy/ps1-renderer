#pragma once

#include <SDL3/SDL.h>

#include "camera.h"
#include "window.h"

#define PLAYER_GRAVITY (vec3) { 0.0f, -1.0f, 0.0f }
#define PLAYER_WALK_SPEED 40.0f
#define PLAYER_RUN_SPEED 55.0f

#define PLAYER_GROUND_RESISTENCE 0.99f

typedef struct Player {
    vec3 position;
    vec3 velocity;
    bool grounded;

    Camera camera;

    float movement_speed;

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
