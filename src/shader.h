#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef GLuint Shader;

Shader shader_create(const char* vertex_shader_source_path, const char* fragment_shader_source_path);
void shader_bind(Shader shader);
void shader_set_mat4_uniform(GLuint uniform_location, mat4 value);
void shader_set_vec3_uniform(GLuint uniform_location, vec3 value);
void shader_set_float_uniform(GLuint uniform_location, GLfloat value);
void shader_set_int_uniform(GLuint uniform_location, GLint value);
void shader_unbind();
void shader_destroy(Shader shader);
