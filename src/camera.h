#pragma once

#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "window.h"

typedef struct Camera {
    mat4 projection;
    mat4 view;

    vec3 position;
    vec3 front;
    vec3 up;

    float fov;

    float pitch;
    float yaw;

    bool space_pressed;
    bool shift_pressed;
    bool w_pressed;
    bool a_pressed;
    bool s_pressed;
    bool d_pressed;
} Camera;

Camera camera_create(Window* window, vec3 position, float fov);
void camera_event(Camera* camera, SDL_Event* event);
void camera_update(Camera* camera, double delta_time);
void camera_destroy(Camera* camera);
