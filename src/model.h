#pragma once

#include <cglm/cglm.h>

#include "node.h"
#include "base-types.h"
#include "texture.h"
#include "shader.h"

typedef struct Model {
	Node* nodes;
	usize nodes_count;

	Texture* textures;
	usize textures_count;

	mat4 transform;
} Model;

Model model_create(const char* model_path);
void model_draw(Model* model, Shader shader);
void model_destroy(Model* model);
