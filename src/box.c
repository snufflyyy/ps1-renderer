#include "box.h"

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"
#include "camera.h"

Box* box_create(vec2 position, vec2 size, Shader shader) {
	Box* box = (Box*) malloc(sizeof(Box));
	if (!box) {
		fprintf(stderr, "[ERROR] [BOX] Failed to allocate memory for box!\n");
		return NULL;
	}

	glm_vec2_copy(position, box->position);
	glm_vec2_copy(size, box->size);
	box->quad = quad_create();

	glm_mat4_identity(box->model);
	glm_translate(box->model, (vec3) { position[0] + (size[0] / 2.0f), position[1] + (size[1] / 2.0f), 0.0f });
	glm_scale(box->model, (vec3) { size[0], size[1], 1.0f });

	box->shader = shader;

	printf("[INFO] [BOX] Created Box\n");
	return box;
}

void box_draw(Box* box, Camera* camera) {
	shader_bind(box->shader);
	shader_set_mat4_uniform(glGetUniformLocation(box->shader, "model"), box->model);
	shader_set_mat4_uniform(glGetUniformLocation(box->shader, "view"), camera->view);
	shader_set_mat4_uniform(glGetUniformLocation(box->shader, "projection"), camera->projection);

	quad_draw(&box->quad);
	shader_unbind();
}

void box_destroy(Box* box) {
	shader_destroy(box->shader);
	quad_destroy(&box->quad);
	free(box);
	printf("[INFO] [BOX] Destroyed Box\n");
}
