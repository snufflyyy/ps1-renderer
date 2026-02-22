#include "camera.h"

#include <math.h>

#include <cglm/cglm.h>
#include <stdbool.h>

#include "SDL3/SDL_scancode.h"
#include "cglm/vec3.h"
#include "window.h"

Camera camera_create(Window* window, vec3 position, float fov) {
    Camera camera = {0};

    u32 window_width, window_height;
    window_get_size(window, &window_width, &window_height);

    glm_perspective(fov, (float) window_width / (float) window_height, 0.1f, 1000.0f, camera.projection);
    glm_mat4_identity(camera.view);

    glm_vec3_copy(position, camera.position);
    camera.fov = fov;

    glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, camera.front);
    glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, camera.up);

    return camera;
}

void camera_event(Camera* camera, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float x_motion = event->motion.xrel;
            float y_motion = -event->motion.yrel;

            const float sensitivity = 0.1f;

            camera->yaw += (x_motion * sensitivity);
            camera->pitch += (y_motion * sensitivity);

            if (camera->pitch > 89.0f) { camera->pitch = 89.0f; }
            if (camera->pitch < -89.0f) { camera->pitch = -89.0f; }

            vec3 direction = {
                cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch)),
                sinf(glm_rad(camera->pitch)),
                sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch)),
            };
            glm_normalize(direction);
            glm_vec3_copy(direction, camera->front);
        } break;
        case SDL_EVENT_KEY_DOWN: {
            switch (event->key.scancode) {
            	case SDL_SCANCODE_SPACE: camera->space_pressed = true; break;
           		case SDL_SCANCODE_LSHIFT: camera->shift_pressed = true; break;
                case SDL_SCANCODE_W: camera->w_pressed = true; break;
                case SDL_SCANCODE_S: camera->s_pressed = true; break;
                case SDL_SCANCODE_A: camera->a_pressed = true; break;
                case SDL_SCANCODE_D: camera->d_pressed = true; break;
                default: break;
            }
        } break;
        case SDL_EVENT_KEY_UP: {
            switch (event->key.scancode) {
          		case SDL_SCANCODE_SPACE: camera->space_pressed = false; break;
           		case SDL_SCANCODE_LSHIFT: camera->shift_pressed = false; break;
                case SDL_SCANCODE_W: camera->w_pressed = false; break;
                case SDL_SCANCODE_S: camera->s_pressed = false; break;
                case SDL_SCANCODE_A: camera->a_pressed = false; break;
                case SDL_SCANCODE_D: camera->d_pressed = false; break;
                default: break;
            }
        } break;
    }
}

void camera_update(Camera* camera, double delta_time) {
    const float camera_speed = 5.0f;

    vec3 flat_front = { cosf(glm_rad(camera->yaw)), 0.0f, sinf(glm_rad(camera->yaw)) };
    glm_normalize(flat_front);

    vec3 right;
    glm_cross(flat_front, camera->up, right);
    glm_normalize(right);

    vec3 move = {0};

    if (camera->w_pressed) {
	    vec3 forward_delta;
	    glm_vec3_scale(flat_front, 1.0f, forward_delta);
	    glm_vec3_add(move, forward_delta, move);
    }
    if (camera->s_pressed) {
	    vec3 backward_delta;
	    glm_vec3_scale(flat_front, -1.0f, backward_delta);
	    glm_vec3_add(move, backward_delta, move);
    }
    if (camera->a_pressed) {
	    vec3 left_delta;
	    glm_vec3_scale(right, -1.0f, left_delta);
	    glm_vec3_add(move, left_delta, move);
    }
    if (camera->d_pressed) {
	    vec3 right_delta;
	    glm_vec3_scale(right, 1.0f, right_delta);
	    glm_vec3_add(move, right_delta, move);
    }
    if (camera->space_pressed) {
    	vec3 up_delta;
	    glm_vec3_scale(camera->up, 1.0f, up_delta);
	    glm_vec3_add(move, up_delta, move);
    }
    if (camera->shift_pressed) {
    	vec3 down_delta;
	    glm_vec3_scale(camera->up, -1.0f, down_delta);
	    glm_vec3_add(move, down_delta, move);
    }

    if (glm_vec3_norm(move) > 0.0f) {
        glm_normalize(move);
        glm_vec3_scale(move, camera_speed * (float) delta_time, move);
        glm_vec3_add(camera->position, move, camera->position);
    }

    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void camera_destroy(Camera* camera) {

}
