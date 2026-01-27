#include "camera.h"
#include "cglm/affine.h"
#include "cglm/mat4.h"
#include "cglm/vec2.h"

#include <cglm/cglm.h>
#include <SDL3/SDL.h>
#include <stdbool.h>

Camera camera_create(u32 width, u32 height) {
	Camera camera = {0};

	camera.zoom = 1.0f;

	glm_mat4_identity(camera.view);
	glm_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, camera.projection);

	printf("[INFO] [CAMERA] Created Camera!\n");
	return camera;
}

void camera_event(Camera* camera, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            camera_resize(camera, event->window.data1, event->window.data2);
		} break;
    }
}

void camera_update(Camera* camera, InputManager* input_manager) {
    if (input_manager->right_mouse_button) {
        glm_vec2_zero(camera->position);
        camera->zoom = 1.0f;

        glm_mat4_identity(camera->view);

        camera->locked = true;
    }

    if (input_manager->middle_mouse_button) {
        camera->locked = false;

        if (!camera->moving) {
            glm_vec2_copy(input_manager->mouse_position, camera->starting_mouse_position);
            camera->moving = true;
        }
        if (camera->moving) {
            vec2 mouse_offset;
            glm_vec2_sub(input_manager->mouse_position, camera->starting_mouse_position, mouse_offset);
            glm_vec2_add(camera->position, mouse_offset, camera->position);
            glm_vec2_copy(input_manager->mouse_position, camera->starting_mouse_position);

            glm_mat4_identity(camera->view);
            glm_translate(camera->view, (vec3) { camera->position[0], camera->position[1], 0.0f });
            glm_scale(camera->view, (vec3) { camera->zoom, camera->zoom, 1.0f });
        }
    } else {
        camera->moving = false;
    }

    if (input_manager->scroll_wheel != 0.0f) {
        if (input_manager->scroll_wheel > 0.0f) {
            camera->zoom += 0.25f;
        } else {
            if (camera->zoom > 0.25f) { camera->zoom -= 0.25f; }
        }

        glm_mat4_identity(camera->view);
        glm_translate(camera->view, (vec3) { camera->position[0], camera->position[1], 0.0f });
        glm_scale(camera->view, (vec3) { camera->zoom, camera->zoom, 1.0f });
    }
}

void camera_resize(Camera* camera, u32 new_width, u32 new_height) {
	glm_ortho(0.0f, new_width, new_height, 0.0f, -1.0f, 1.0f, camera->projection);
}

void camera_destroy(Camera* camera) {
	printf("[INFO] [CAMERA] Camera Destroyed!\n");
}
