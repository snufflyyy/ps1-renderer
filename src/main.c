#include "SDL3/SDL_events.h"
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

	bool show_debug_options = true;
    while (window->running) {
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			window_event(window, &event);
			player_event(&player, &event);

			switch (event.type) {
			    case SDL_EVENT_KEY_DOWN: {
					switch (event.key.scancode) {
					    case SDL_SCANCODE_F10: show_debug_options = !show_debug_options; break;
						default: break;
					}
				} break;
		    }
	    }

		window_update(window);

		if (show_debug_options) {
    		igBegin("Debug Options", &show_debug_options, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
                igText("Toggle debug options by pressing F10");
		        igSeparatorText("Rendering");
				igText("FPS: %0.2f", window->fps);
				igText("Delta time: %f", window->delta_time);
				igText("Fullscreen: %s", (window->fullscreen) ? "Yes" : "No");
      		    igSeparatorText("World");
                igColorEdit3("Sky Color", sky_color, ImGuiColorEditFlags_None);
          		igDragFloat("Fog Density", &fog_density, 0.001f, 0.0f, 1.0f, "%0.3f", ImGuiSliderFlags_None);
                igSeparatorText("Camera");
          		igDragFloat("Player Height Offset", &player.camera_height_offset, 0.01f, 0.0f, 5.0f, "%0.2f", ImGuiSliderFlags_None);
                if (igDragFloat("FOV", &player.camera.fov, 0.1f, 0.0f, 120.0f, "%0.2f", ImGuiSliderFlags_None)) {
                    u32 window_width, window_height;
                    window_get_size(window, &window_width, &window_height);
                    glm_perspective(glm_rad(player.camera.fov), (float) window_width / (float) window_height, player.camera.near, player.camera.far, player.camera.projection);
                }
                igSeparatorText("Player");
      		    igDragFloat3("Position", player.position, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
          		igDragFloat3("Velocity", player.velocity, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
          		igDragFloat3("Gravity", player.gravity, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
                igText("Grounded: %s", (player.grounded) ? "Yes" : "No");
                igCheckbox("Can Run", &player.can_run);
                igCheckbox("Can Jump", &player.can_jump);
          		igDragFloat("Walk Speed", &player.walk_speed, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
          		igDragFloat("Run Speed", &player.run_speed, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
          		igDragFloat("Jump Amount", &player.jump_amount, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igEnd();
		}

		player_update(&player, window->delta_time);

		window_set_clear_color(sky_color);
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
