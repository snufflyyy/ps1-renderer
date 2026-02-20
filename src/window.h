#pragma once

#include <glad/glad.h>
#include <SDL3/SDL.h>

#include "base-types.h"

typedef struct Window {
	SDL_Window* sdl_window;
	SDL_GLContext gl_context;

	bool running;

	u64 last_performance_counter;
	double performance_frequency;

	float fps;
	double delta_time;
} Window;

Window* window_create(u32 width, u32 height, const char* title);
void window_update(Window* window);
void window_event(Window* window, SDL_Event* event);
void window_set_clear_color(float red, float green, float blue);
void window_clear(void);
void window_resize(u32 new_width, u32 new_height);
void window_get_size(Window* window, u32* width, u32* height);
void window_destroy(Window* window);
