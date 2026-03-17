#pragma once

#include <cglm/cglm.h>

#include "gfx/window.h"
#include "network/client/client.h"
#include "network/packet.h"
#include "player.h"
#include "gfx/model.h"
#include "gfx/shader.h"

#define GAME_DEFAULT_SKY_COLOR (vec3) { 0.3f, 0.3f, 0.3f }
#define GAME_DEFAULT_FOG_DENSITY 0.1f

#define GAME_DEBUG_MAX_SERVER_ADDRESS_LENGTH 512
#define GAME_DEBUG_DEFAULT_SERVER_ADDRESS "127.0.0.1:1126"

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
	char debug_server_address_string[GAME_DEBUG_MAX_SERVER_ADDRESS_LENGTH];
	char debug_client_chat_input[PACKET_MAX_DATA_SIZE];
} Game;

Game game_create(void);
void game_update(Game* game);
void game_draw(Game* game);
void game_destroy(Game* game);
