#pragma once

#include <cglm/cglm.h>

#include "mesh.h"
#include "base-types.h"
#include "shader.h"

typedef struct Node {
	Mesh* meshes;
	usize meshes_count;

	mat4 transform;
} Node;

Node* node_create(Mesh* meshes, usize meshes_count, mat4 transform);
void node_draw(Node* node, mat4 parent_transform, Shader shader);
void node_destroy(Node* node);
