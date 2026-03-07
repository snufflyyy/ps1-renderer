#pragma once

#include <cglm/cglm.h>

#include "node.h"
#include "utils/base-types.h"
#include "texture.h"
#include "shader.h"

#define MODEL_MAX_LOADED_TEXTURE_PATH 512

typedef struct LoadedTexture {
    Texture texture;
    char path[MODEL_MAX_LOADED_TEXTURE_PATH];
} LoadedTexture;

typedef struct Model {
	Node** nodes;
	usize nodes_count;

	LoadedTexture* loaded_textures;
	usize loaded_textures_count;
	char root_path[MODEL_MAX_LOADED_TEXTURE_PATH];

	mat4 transform;
} Model;

Model model_create(const char* model_path);
void model_draw(Model* model, Shader shader);
void model_destroy(Model* model);
