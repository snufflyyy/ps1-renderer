#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#include <stdbool.h>

#include "model.h"
#include "shader.h"
#include "window.h"
#include "camera.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main(void) {
	Window* window = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "PS1 Renderer");
	Camera camera = camera_create(window, (vec3) { 0.0f, 1.0f, 0.0f }, 65.0f);

	Shader shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");
	Model school = model_create("../assets/school/scene.gltf");

	window_set_clear_color(0.4f, 0.4f, 0.4f);
    while (window->running) {
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			window_event(window, &event);
			camera_event(&camera, &event);
	    }

		window_update(window);
		camera_update(&camera, window->delta_time);

		printf("FPS: %0.2f\n", window->fps);

		window_clear();

		shader_bind(shader);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "view"), camera.view);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "projection"), camera.projection);

		model_draw(&school, shader);
    }

    model_destroy(&school);
	shader_destroy(shader);
	window_destroy(window);
    return 0;
}
