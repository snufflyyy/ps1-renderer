#pragma once

#include <SDL3/SDL.h>

#include "camera.h"
#include "window.h"

#define PLAYER_GRAVITY (vec3) { 0.0f, -25.0f, 0.0f }

#define PLAYER_WALK_SPEED 25.0f
#define PLAYER_RUN_SPEED 50.0f
#define PLAYER_JUMP_AMOUNT 5.0f

#define PLAYER_MOVEMENT_DAMPING 0.95f

typedef struct Player {
    vec3 position;
    vec3 velocity;
    bool grounded;

    Camera camera;
    float camera_height_offset;

    float walk_speed;

    bool can_run;
    float run_speed;

    float movement_speed;

    float jump_amount;

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
