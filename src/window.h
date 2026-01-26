#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "basic-types.h"

typedef struct Window {
	SDL_Window* sdl_window;
	SDL_GLContext gl_context;

	bool running;
} Window;

Window* window_create(u32 width, u32 height, const char* title);
void window_update(Window* window);
void window_clear(Window* window);
void window_resize(Window* window, u32 new_width, u32 new_height);
void window_get_size(Window* window, u32* width, u32* height);
void window_destroy(Window* window);
