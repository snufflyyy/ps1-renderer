#pragma once

#include <glad/glad.h>

typedef GLuint Texture;

Texture texture_create(const char* image_path);
void texture_bind(GLenum active_texture, Texture texture);
void texture_unbind(void);
void texture_destroy(Texture texture);
