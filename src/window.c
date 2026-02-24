#include "window.h"
#include "SDL3/SDL_scancode.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <cglm/cglm.h>

#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui.h>
#include <cimgui_impl.h>

Window* window_create(u32 width, u32 height, const char* title) {
	Window* window = (Window*) malloc(sizeof(Window));
	if (!window) {
		fprintf(stderr, "[ERROR] [WINDOW] Failed to allocate memory for window!\n");
		return NULL;
	}

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "[ERROR] [WINDOW] Failed to initialize SDL!\n");
		free(window);
		return NULL;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    #ifdef __APPLE__
   		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    #endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

   	window->sdl_window = SDL_CreateWindow(title, (int) width, (int) height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window->sdl_window) {
   		fprintf(stderr, "[ERROR] [WINDOW] Failed to create SDL window!\n");
    	SDL_Quit();
       	free(window);
       	return NULL;
    }

    window->gl_context = SDL_GL_CreateContext(window->sdl_window);
    if (!window->gl_context) {
   		fprintf(stderr, "[ERROR] [WINDOW] Failed to create OpenGL Context!\n");
    	SDL_DestroyWindow(window->sdl_window);
    	SDL_Quit();
       	free(window);
       	return NULL;
    }

    SDL_GL_MakeCurrent(window->sdl_window, window->gl_context);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
  		fprintf(stderr, "[ERROR] [WINDOW] Failed load GLAD!\n");
    	SDL_GL_DestroyContext(window->gl_context);
    	SDL_DestroyWindow(window->sdl_window);
    	SDL_Quit();
       	free(window);
       	return NULL;
    }

    window->mouse_captured = true;
    SDL_SetWindowRelativeMouseMode(window->sdl_window, window->mouse_captured);
    SDL_SetWindowMouseGrab(window->sdl_window, window->mouse_captured);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    window->running = true;
    window->fullscreen = false;

    window->last_performance_counter = SDL_GetPerformanceCounter();
    window->performance_frequency = (double) SDL_GetPerformanceFrequency();

    window->fps = 0.0f;
    window->delta_time = 0.0;

    igCreateContext(NULL);
    ImGuiIO* ioptr = igGetIO_Nil();
    ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL3_InitForOpenGL(window->sdl_window, window->gl_context);
    ImGui_ImplOpenGL3_Init("#version 410 core");

    printf("[INFO] [WINDOW] Created Window:\n");
    printf("\tVideo Driver: %s\n", SDL_GetCurrentVideoDriver());
    printf("\tVender: %s\n", glGetString(GL_VENDOR));
    printf("\tRenderer: %s\n", glGetString(GL_RENDERER));
    printf("\tOpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	return window;
}

void window_event(Window* window, SDL_Event* event) {
    switch (event->type) {
        case SDL_EVENT_QUIT: { window->running = false; } break;
        case SDL_EVENT_KEY_DOWN: {
            switch (event->key.scancode) {
                case SDL_SCANCODE_F11: {
                    SDL_SetWindowFullscreen(window->sdl_window, !window->fullscreen);
                    window->fullscreen = !window->fullscreen;
                } break;
                case SDL_SCANCODE_ESCAPE: {
                    SDL_SetWindowRelativeMouseMode(window->sdl_window, !window->mouse_captured);
                    SDL_SetWindowMouseGrab(window->sdl_window, !window->mouse_captured);
                    window->mouse_captured = !window->mouse_captured;
                } break;
                default: break;
            }
        } break;
		case SDL_EVENT_WINDOW_RESIZED: {
			window_resize((u32) event->window.data1, (u32) event->window.data2);
		} break;
    }

    if (!window->mouse_captured) { ImGui_ImplSDL3_ProcessEvent(event); }
}

void window_update(Window* window) {
	SDL_GL_SwapWindow(window->sdl_window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    igNewFrame();

	u64 performance_counter = SDL_GetPerformanceCounter();
	window->delta_time = (double) (performance_counter - window->last_performance_counter) / window->performance_frequency;
	window->last_performance_counter = performance_counter;

	if (window->delta_time > 0.0) {
	    window->fps = 1.0f / ((float) window->delta_time);
	}
}

void window_imgui_draw(Window* window) {
    igEndFrame();
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void window_set_clear_color(vec3 color) {
    glClearColor(color[0], color[1], color[2], 1.0f);
}

void window_clear(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_resize(u32 new_width, u32 new_height) {
	glViewport(0, 0, (GLsizei) new_width, (GLsizei) new_height);
	printf("[INFO] [WINDOW] Window Resized to %ux%u\n", new_width, new_height);
}

void window_get_size(Window* window, u32* width, u32* height) {
	SDL_GetWindowSize(window->sdl_window, (int*) width, (int*) height);
}

void window_destroy(Window* window) {
	window->running = false;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    igDestroyContext(NULL);
 	SDL_GL_DestroyContext(window->gl_context);
   	SDL_DestroyWindow(window->sdl_window);
   	SDL_Quit();
    free(window);

    printf("[INFO] [WINDOW] Window Destroyed\n");
}
