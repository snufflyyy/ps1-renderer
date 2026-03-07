#pragma once

#include <cglm/cglm.h>

#include "gfx/mesh.h"
#include "gfx/shader.h"

typedef struct Cube {
    Mesh mesh;
    mat4 transform;
} Cube;

Cube cube_create(const char* texture_path);
void cube_update(Cube* cube);
void cube_draw(Cube* cube, Shader shader);
void cube_destroy(Cube* cube);
