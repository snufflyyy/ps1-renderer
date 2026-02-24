#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#include <stdbool.h>

#include "model.h"
#include "shader.h"
#include "window.h"
#include "player.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_CLEAR_COLOR (vec3) { 0.3f, 0.3f, 0.3f }

int main(void) {
	Window* window = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "PS1 Renderer");

	Shader shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");
	Model school = model_create("../assets/school/scene.gltf");

	Player player = player_create((vec3) { 0.0f, 0.0f, 0.0f }, window);

	window_set_clear_color(WINDOW_CLEAR_COLOR[0], WINDOW_CLEAR_COLOR[1], WINDOW_CLEAR_COLOR[2]);
    while (window->running) {
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			window_event(window, &event);
			player_event(&player, &event);
	    }

		window_update(window);
		player_update(&player, window->delta_time);

		window_clear();

		shader_bind(shader);

		shader_set_mat4_uniform(glGetUniformLocation(shader, "view"), player.camera.view);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "projection"), player.camera.projection);
		shader_set_float_uniform(glGetUniformLocation(shader, "far"), player.camera.far);
		shader_set_float_uniform(glGetUniformLocation(shader, "near"), player.camera.near);

		shader_set_vec3_uniform(glGetUniformLocation(shader, "fog_color"), WINDOW_CLEAR_COLOR);
		shader_set_float_uniform(glGetUniformLocation(shader, "fog_start"), 1.0f);
		shader_set_float_uniform(glGetUniformLocation(shader, "fog_end"), 25.0f);

		model_draw(&school, shader);
    }

    player_destroy(&player);
    model_destroy(&school);
	shader_destroy(shader);
	window_destroy(window);
    return 0;
}
