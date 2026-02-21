#include "camera.h"

#include <math.h>

#include "SDL3/SDL_scancode.h"
#include "cglm/affine.h"
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
                case SDL_SCANCODE_W: camera->y_movement = 1; break;
                case SDL_SCANCODE_S: camera->y_movement = -1; break;
                case SDL_SCANCODE_A: camera->x_movement = -1; break;
                case SDL_SCANCODE_D: camera->x_movement = 1; break;
                default: break;
            }
        } break;
        case SDL_EVENT_KEY_UP: {
            switch (event->key.scancode) {
                case SDL_SCANCODE_W: camera->y_movement = 0; break;
                case SDL_SCANCODE_S: camera->y_movement = 0; break;
                case SDL_SCANCODE_A: camera->x_movement = 0; break;
                case SDL_SCANCODE_D: camera->x_movement = 0; break;
                default: break;
            }
        } break;
    }
}

void camera_update(Camera* camera) {
    const float camera_speed = 0.05f;

    vec3 new_camera_delta;
    if (camera->y_movement == 1) {
        glm_vec3_scale(camera->front, camera_speed, new_camera_delta);
        glm_vec3_add(camera->position, new_camera_delta, camera->position);
    } else if (camera->y_movement == -1) {
        glm_vec3_scale(camera->front, camera_speed, new_camera_delta);
        glm_vec3_sub(camera->position, new_camera_delta, camera->position);
    }

    if (camera->x_movement == -1) {
        glm_cross(camera->front, camera->up, new_camera_delta);
        glm_vec3_scale(new_camera_delta, camera_speed, new_camera_delta);
        glm_normalize(new_camera_delta);
        glm_vec3_sub(camera->position, new_camera_delta, camera->position);
    } else if (camera->x_movement == 1) {
        glm_cross(camera->front, camera->up, new_camera_delta);
        glm_vec3_scale(new_camera_delta, camera_speed, new_camera_delta);
        glm_normalize(new_camera_delta);
        glm_vec3_add(camera->position, new_camera_delta, camera->position);
    }

    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void camera_destroy(Camera* camera) {

}
