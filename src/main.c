#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#include <stdbool.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#include "model.h"
#include "shader.h"
#include "window.h"
#include "player.h"

int main(void) {
	Window* window = window_create(1280, 720, "PS1 Renderer");

	Shader shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");
	Model school = model_create("../assets/school/scene.gltf");

	Player player = player_create((vec3) { 0.0f, 0.0f, 0.0f }, window);

	vec3 sky_color = { 0.3f, 0.3f, 0.3f };
	float fog_density = 0.1f;

    while (window->running) {
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			window_event(window, &event);
			player_event(&player, &event);
	    }

		window_update(window);

  		igBegin("Options", NULL, ImGuiWindowFlags_None);
  		    igSeparatorText("World");
            igColorEdit3("Sky Color", sky_color, ImGuiColorEditFlags_None);
      		igDragFloat("Fog Density", &fog_density, 0.001f, 0.0f, 1.0f, "%0.3f", ImGuiSliderFlags_None);
            igSeparatorText("Camera");
      		igDragFloat("Player Height Offset", &player.camera_height_offset, 0.01f, 0.0f, 5.0f, "%0.2f", ImGuiSliderFlags_None);
            igDragFloat("FOV", &player.camera.fov, 0.01f, 0.0f, 120.0f, "%0.2f", ImGuiSliderFlags_None);
            igSeparatorText("Player");
  		    igDragFloat3("Position", player.position, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat3("Velocity", player.velocity, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat("Walk Speed", &player.walk_speed, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igCheckbox("Can Run", &player.can_run);
      		igDragFloat("Run Speed", &player.run_speed, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat("Jump Amount", &player.jump_amount, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
  		igEnd();

		player_update(&player, window->delta_time);

		window_set_clear_color(sky_color[0], sky_color[1], sky_color[2]);
		window_clear();

		shader_bind(shader);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "view"), player.camera.view);
		shader_set_mat4_uniform(glGetUniformLocation(shader, "projection"), player.camera.projection);
		shader_set_float_uniform(glGetUniformLocation(shader, "far"), player.camera.far);
		shader_set_float_uniform(glGetUniformLocation(shader, "near"), player.camera.near);

		shader_set_vec3_uniform(glGetUniformLocation(shader, "fog_color"), sky_color);
		shader_set_float_uniform(glGetUniformLocation(shader, "fog_density"), fog_density);

		model_draw(&school, shader);
		window_imgui_draw(window);
    }

    player_destroy(&player);
    model_destroy(&school);
	shader_destroy(shader);
	window_destroy(window);
    return 0;
}
