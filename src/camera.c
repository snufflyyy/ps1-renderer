#include "camera.h"
#include "cglm/cam.h"

#include <cglm/cglm.h>

Camera camera_create(u32 width, u32 height) {
	Camera camera = {0};

	glm_mat4_identity(camera.view);
	glm_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, camera.projection);

	printf("[INFO] [CAMERA] Created Camera!\n");
	return camera;
}

void camera_resize(Camera* camera, u32 new_width, u32 new_height) {
	glm_ortho(0.0f, new_width, new_height, 0.0f, -1.0f, 1.0f, camera->projection);
}

void camera_destroy(Camera* camera) {
	printf("[INFO] [CAMERA] Camera Destroyed!\n");
}
