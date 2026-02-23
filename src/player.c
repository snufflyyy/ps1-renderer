#include "player.h"
#include "camera.h"
#include "cglm/vec3.h"

Player player_create(vec3 position, Window* window) {
    Player player = {0};

    vec3 camera_position;
    glm_vec3_copy((vec3) { position[0], position[1] + 1.0f, position[2] }, camera_position);
    player.camera = camera_create(window, camera_position, 65.0f);

    player.movement_speed = 5.0f;

    return player;
}

void player_event(Player* player, SDL_Event* event) {
    camera_event(&player->camera, event);

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event->key.scancode) {
            	case SDL_SCANCODE_SPACE: player->space_pressed = true; break;
           		case SDL_SCANCODE_LSHIFT: player->shift_pressed = true; break;
                case SDL_SCANCODE_W: player->w_pressed = true; break;
                case SDL_SCANCODE_S: player->s_pressed = true; break;
                case SDL_SCANCODE_A: player->a_pressed = true; break;
                case SDL_SCANCODE_D: player->d_pressed = true; break;
                default: break;
            }
        } break;
        case SDL_EVENT_KEY_UP: {
            switch (event->key.scancode) {
          		case SDL_SCANCODE_SPACE: player->space_pressed = false; break;
           		case SDL_SCANCODE_LSHIFT: player->shift_pressed = false; break;
                case SDL_SCANCODE_W: player->w_pressed = false; break;
                case SDL_SCANCODE_S: player->s_pressed = false; break;
                case SDL_SCANCODE_A: player->a_pressed = false; break;
                case SDL_SCANCODE_D: player->d_pressed = false; break;
                default: break;
            }
        } break;
    }
}

void player_update(Player* player, double delta_time) {
    vec3 flat_front = { cosf(glm_rad(player->camera.yaw)), 0.0f, sinf(glm_rad(player->camera.yaw)) };
    glm_normalize(flat_front);

    vec3 right;
    glm_cross(flat_front, player->camera.up, right);
    glm_normalize(right);

    vec3 move = {0};

    if (player->w_pressed) {
	    vec3 forward_delta;
	    glm_vec3_scale(flat_front, 1.0f, forward_delta);
	    glm_vec3_add(move, forward_delta, move);
    }
    if (player->s_pressed) {
	    vec3 backward_delta;
	    glm_vec3_scale(flat_front, -1.0f, backward_delta);
	    glm_vec3_add(move, backward_delta, move);
    }
    if (player->a_pressed) {
	    vec3 left_delta;
	    glm_vec3_scale(right, -1.0f, left_delta);
	    glm_vec3_add(move, left_delta, move);
    }
    if (player->d_pressed) {
	    vec3 right_delta;
	    glm_vec3_scale(right, 1.0f, right_delta);
	    glm_vec3_add(move, right_delta, move);
    }
    if (player->space_pressed) {
    	vec3 up_delta;
	    glm_vec3_scale(player->camera.up, 1.0f, up_delta);
	    glm_vec3_add(move, up_delta, move);
    }
    if (player->shift_pressed) {
    	vec3 down_delta;
	    glm_vec3_scale(player->camera.up, -1.0f, down_delta);
	    glm_vec3_add(move, down_delta, move);
    }

    if (glm_vec3_norm(move) > 0.0f) {
        glm_normalize(move);
        glm_vec3_scale(move, player->movement_speed * (float) delta_time, move);
        glm_vec3_add(player->position, move, player->position);
    }

    vec3 center;
    glm_vec3_add(player->position, player->camera.front, center);
    glm_lookat(player->position, center, player->camera.up, player->camera.view);

    glm_vec3_copy((vec3) {player->position[0], player->position[1] + 1.0f, player->position[2] }, player->camera.position);

    camera_update(&player->camera, delta_time);
}

void player_destroy(Player* player) {
    camera_destroy(&player->camera);
}
