#pragma once

#include <glad/glad.h>

typedef struct Quad {
	GLuint vao, vbo, ebo;
} Quad;

Quad quad_create();
void quad_draw(Quad* quad);
void quad_destroy(Quad* quad);
