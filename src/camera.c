#include "camera.h"

#include <math.h>

#include <cglm/cglm.h>
#include <stdbool.h>

#include "cglm/vec3.h"
#include "window.h"

Camera camera_create(Window* window, vec3 position, float fov) {
    Camera camera = {0};

    u32 window_width, window_height;
    window_get_size(window, &window_width, &window_height);

    glm_perspective(glm_rad(fov), (float) window_width / (float) window_height, 0.1f, 1000.0f, camera.projection);
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
    }
}

void camera_update(Camera* camera, double delta_time) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, camera->view);
}

void camera_destroy(Camera* camera) {

}
