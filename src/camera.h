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

    float far;
    float near;

    float pitch;
    float yaw;
} Camera;

Camera camera_create(Window* window, vec3 position, float fov);
void camera_event(Camera* camera, SDL_Event* event);
void camera_update(Camera* camera);
void camera_destroy(Camera* camera);
