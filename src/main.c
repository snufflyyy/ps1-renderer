#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#include "camera.h"
#include "input.h"
#include "text.h"
#include "window.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main() {
	Window* window = window_create(WINDOW_WIDTH, WINDOW_HEIGHT, "SeaChess");
	Camera camera = camera_create(WINDOW_WIDTH, WINDOW_HEIGHT);
	InputManager input_manager = {0};
	FontManager font_manager = font_manager_create();

	Font font = font_create(&font_manager, 24, "../fonts/IBM Plex Sans/IBMPlexSans-Regular.ttf");

	window_set_clear_color(window, 0.14f, 0.14f, 0.14f);

    while (window->running) {
        input_manager_update(&input_manager);
    	SDL_Event event;
	    while (SDL_PollEvent(&event)) {
			window_event(window, &event);
			camera_event(&camera, &event);
			input_manager_event(&input_manager, &event);
	    }
		window_update(window);
		camera_update(&camera, &input_manager);

		window_clear(window);
		text_draw(&camera, &font, &font_manager, (vec2) { 0.0f, 0.0f }, "SeaChess");
    }

    font_destroy(&font);
    font_manager_destroy(&font_manager);
    camera_destroy(&camera);
    window_destroy(window);

    return 0;
}
