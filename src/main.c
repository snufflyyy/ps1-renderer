#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "cglm/affine-pre.h"
#include "cglm/mat4.h"
#include "cglm/util.h"
#include "model.h"
#include "shader.h"
#include "window.h"
#include "vertex.h"
#include "mesh.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main(void) {
	Window* window = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "PS1 Renderer");

	Shader shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");
	Model backpack = model_create("../assets/backpack/scene.gltf");

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	glm_perspective(65.0f, ((float) WINDOW_WIDTH) / WINDOW_HEIGHT, 0.1f, 1000.0f, projection);

	mat4 view = GLM_MAT4_IDENTITY_INIT;
	glm_translate(view, (vec3) { 0.0f, 0.0f, -250.0f });

	float angle = 0.0f;

	window_set_clear_color(0.0f, 0.0f, 0.0f);
    while (window->running) {
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			window_event(window, &event);
	    }

		glm_mat4_identity(backpack.transform);
		glm_rotate(backpack.transform, angle, (vec3) { 0.0f, 1.0f, 0.0f });
		angle += 1.0f * (float) window->delta_time;

		window_update(window);
		window_clear();

		shader_bind(shader);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "view"), view);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "projection"), projection);

		model_draw(&backpack, shader);
    }

    model_destroy(&backpack);
	shader_destroy(shader);
	window_destroy(window);
    return 0;
}
