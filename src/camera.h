#pragma once

#include <cglm/cglm.h>
#include <SDL3/SDL.h>

#include "base-types.h"
#include "input.h"

typedef struct Camera {
    vec2 position;
    float zoom;

	mat4 view;
	mat4 projection;

	bool locked;

	bool moving;
	vec2 starting_mouse_position;
} Camera;

Camera camera_create(u32 max_width, u32 max_height);
void camera_event(Camera* camera, SDL_Event* event);
void camera_update(Camera* camera, InputManager* input_manager);
void camera_resize(Camera* camera, u32 new_width, u32 new_height);
void camera_destroy(Camera* camera);
