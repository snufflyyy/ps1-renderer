#pragma once

#include <glad/glad.h>

typedef GLuint Texture;

Texture texture_create();

void texture_bind(Texture texture);
void texture_unbind();
void texture_destroy(Texture texture);
