#pragma once

#include <cglm/cglm.h>

#include "basic-types.h"

typedef struct Camera {
	mat4 view;
	mat4 projection;
} Camera;

Camera camera_create(u32 max_width, u32 max_height);
void camera_resize(Camera* camera, u32 new_width, u32 new_height);
void camera_destroy(Camera* camera);
