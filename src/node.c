#include "node.h"
#include "cglm/mat4.h"
#include "mesh.h"
#include "shader.h"

#include <stdio.h>

#include <cglm/cglm.h>

Node* node_create(Mesh* meshes, usize meshes_count, mat4 transform) {
	Node* node = (Node*) malloc(sizeof(Node));
	if (!node) {
		fprintf(stderr, "[ERROR] [NODE] Failed to allocate memory for node!\n");
		return NULL;
	}

	node->meshes = meshes;
	node->meshes_count = meshes_count;
	glm_mat4_copy(transform, node->transform);

	return node;
}

void node_draw(Node* node, mat4 parent_transform, Shader shader) {
	mat4 transform;
	glm_mat4_mul(parent_transform, node->transform, transform);

	shader_set_mat4_uniform(glGetUniformLocation(shader, "model"), transform);
	for (usize i = 0; i < node->meshes_count; i++) {
		mesh_draw(&node->meshes[i]);
	}
}

void node_destroy(Node* node) {
	for (usize i = 0; i < node->meshes_count; i++) {
		mesh_destroy(&node->meshes[i]);
	}
}
