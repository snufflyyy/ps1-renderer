#pragma once

#include <cglm/cglm.h>

#include "quad.h"
#include "shader.h"
#include "camera.h"

typedef struct Box {
	mat4 model;
	vec2 position;
	vec2 size;

	Quad quad;
	Shader shader;
} Box;

Box* box_create(vec2 position, vec2 size, Shader shader);
void box_draw(Box* box, Camera* camera);
void box_destroy(Box* box);
