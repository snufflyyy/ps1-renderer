#pragma once

#include <cglm/cglm.h>

#include "gfx/window.h"
#include "network/client/client.h"
#include "player.h"
#include "gfx/model.h"
#include "gfx/shader.h"

#define GAME_DEFAULT_SKY_COLOR (vec3) { 0.3f, 0.3f, 0.3f }
#define GAME_DEFAULT_FOG_DENSITY 0.1f

typedef struct Game {
	Window* window;
	Client* client;

	Player player;

	Model school;
	Shader school_shader;

	vec3 sky_color;
	float fog_density;

	bool running;
	bool show_debug_options;
} Game;

Game game_create(void);
void game_update(Game* game);
void game_draw(Game* game);
void game_destroy(Game* game);
