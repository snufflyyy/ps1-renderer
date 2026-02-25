#include "game.h"

#include <cglm/cglm.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>

#include "model.h"
#include "window.h"
#include "player.h"

static void game_event(Game* game, SDL_Event* event);

Game game_create(void) {
	Game game = {0};

	game.window = window_create(1280, 720, "PS1 Renderer");
	game.player = player_create((vec3) { 0.0f, 0.0f, 0.0f }, game.window);

	game.school = model_create("../assets/school/scene.gltf");
	game.school_shader = shader_create("../shaders/shader.vert", "../shaders/shader.frag");

	glm_vec3_copy(GAME_DEFAULT_SKY_COLOR, game.sky_color);
	game.fog_density = GAME_DEFAULT_FOG_DENSITY;

	game.running = true;
	game.show_debug_options = true;

	return game;
}

void game_update(Game* game) {
	SDL_Event event;
 	while (SDL_PollEvent(&event)) { game_event(game, &event); }
 	window_update(game->window);
 	game->running = game->window->running;
 	player_update(&game->player, game->window->delta_time);

	if (game->show_debug_options) {
    	igBegin("Debug Options", &game->show_debug_options, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize);
            igText("Toggle debug options by pressing F10");
	        igSeparatorText("Rendering");
			igText("FPS: %0.2f", game->window->fps);
			igText("Delta time: %f", game->window->delta_time);
			if (igCheckbox("Fullscreen", &game->window->fullscreen)) { window_set_fullscreen(game->window, game->window->fullscreen); }
			if (igCheckbox("Vsync", &game->window->vsync)) { window_set_vsync(game->window, game->window->vsync); }
  		    igSeparatorText("World");
            igColorEdit3("Sky Color", game->sky_color, ImGuiColorEditFlags_None);
      		igDragFloat("Fog Density", &game->fog_density, 0.001f, 0.0f, 1.0f, "%0.3f", ImGuiSliderFlags_None);
            igSeparatorText("Camera");
      		igDragFloat("Player Height Offset", &game->player.camera_height_offset, 0.01f, 0.0f, 5.0f, "%0.2f", ImGuiSliderFlags_None);
            if (igDragFloat("FOV", &game->player.camera.fov, 0.1f, 0.0f, 120.0f, "%0.2f", ImGuiSliderFlags_None)) {
                u32 window_width, window_height;
                window_get_size(game->window, &window_width, &window_height);
                glm_perspective(glm_rad(game->player.camera.fov), (float) window_width / (float) window_height, game->player.camera.near, game->player.camera.far, game->player.camera.projection);
            }
            igSeparatorText("Player");
  		    igDragFloat3("Position", game->player.position, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat3("Velocity", game->player.velocity, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat3("Gravity", game->player.gravity, 0.01f, -1000.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
            igText("Grounded: %s", (game->player.grounded) ? "Yes" : "No");
            igCheckbox("Can Run", &game->player.can_run);
            igCheckbox("Can Jump", &game->player.can_jump);
      		igDragFloat("Walk Speed", &game->player.walk_speed, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat("Run Speed", &game->player.run_speed, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
      		igDragFloat("Jump Amount", &game->player.jump_amount, 0.01f, 0.0f, 1000.0f, "%0.2f", ImGuiSliderFlags_None);
  		igEnd();
	}
}

static void game_event(Game* game, SDL_Event* event) {
	window_event(game->window, event);
   	player_event(&game->player, event);

   	switch (event->type) {
  		case SDL_EVENT_KEY_DOWN: {
 			switch (event->key.scancode) {
     			case SDL_SCANCODE_F10: game->show_debug_options = !game->show_debug_options; break;
      			default: break;
        	}
       	} break;
   	}
}

void game_draw(Game* game) {
	window_set_clear_color(game->sky_color);
	window_clear();

	shader_bind(game->school_shader);
	shader_set_mat4_uniform(glGetUniformLocation(game->school_shader, "view"), game->player.camera.view);
	shader_set_mat4_uniform(glGetUniformLocation(game->school_shader, "projection"), game->player.camera.projection);
	shader_set_float_uniform(glGetUniformLocation(game->school_shader, "far"), game->player.camera.far);
	shader_set_float_uniform(glGetUniformLocation(game->school_shader, "near"), game->player.camera.near);
	shader_set_vec3_uniform(glGetUniformLocation(game->school_shader, "fog_color"), game->sky_color);
	shader_set_float_uniform(glGetUniformLocation(game->school_shader, "fog_density"), game->fog_density);
	model_draw(&game->school, game->school_shader);
	window_imgui_draw(game->window);
}

void game_destroy(Game* game) {
    shader_destroy(game->school_shader);
    model_destroy(&game->school);
	player_destroy(&game->player);
    window_destroy(game->window);
}
