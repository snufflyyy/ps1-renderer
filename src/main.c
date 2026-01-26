#include <stdio.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "window.h"
#include "shader.h"
#include "box.h"

int main() {
	Window* window = window_create(1280, 720, "SeaChess");
	Camera camera = camera_create(1280, 720);

	Box* box = box_create((vec2) { 1280.0f / 2.0f - 50.0f, 720.0f / 2.0f - 50.0f }, (vec2) { 100.0f, 100.0f }, shader_create("../shaders/shader.vert", "../shaders/shader.frag"));

    while (window->running) {
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_EVENT_QUIT: window->running = false; break;
				case SDL_EVENT_WINDOW_RESIZED: {
					window_resize(window, event.window.data1, event.window.data2);
					camera_resize(&camera, event.window.data1, event.window.data2);
				} break;
			}
	    }
		window_update(window);

		window_clear(window);
		box_draw(box, &camera);
    }

    box_destroy(box);
    camera_destroy(&camera);
    window_destroy(window);

    return 0;
}
