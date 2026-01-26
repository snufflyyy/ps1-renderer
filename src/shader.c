#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "file.h"

static GLuint shader_create_module(const char* shader_source_path, GLint shader_type);

Shader shader_create(const char* vertex_shader_source_path, const char* fragment_shader_source_path) {
    Shader shader;

    GLuint vertex_shader = shader_create_module(vertex_shader_source_path, GL_VERTEX_SHADER);
    GLuint fragment_shader = shader_create_module(fragment_shader_source_path, GL_FRAGMENT_SHADER);

    shader = glCreateProgram();
    glAttachShader(shader, vertex_shader);
    glAttachShader(shader, fragment_shader);
    glLinkProgram(shader);

    int shader_program_link_status;
    glGetProgramiv(shader, GL_LINK_STATUS, &shader_program_link_status);
    if (shader_program_link_status == GL_FALSE) {
        char info_log[512];
        glGetProgramInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "[ERROR] [SHADER] Failed to link shader program:\n%s\n", info_log);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(shader);
        return shader;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    printf("[INFO] [SHADER] Created shader!\n");
    return shader;
}

static GLuint shader_create_module(const char* shader_source_path, GLint shader_type) {
    const char* shader_source = file_open(shader_source_path);
    if (!shader_source) {
        fprintf(stderr, "[ERROR] [SHADER] Failed to open shader source file!\n");
        return 0;
    }

    GLuint shader_module = glCreateShader(shader_type);
    glShaderSource(shader_module, 1, &shader_source, NULL);
    glCompileShader(shader_module);

    GLint shader_compile_result;
    glGetShaderiv(shader_module, GL_COMPILE_STATUS, &shader_compile_result);
    if (shader_compile_result == GL_FALSE) {
        char info_log[512];
        glGetShaderInfoLog(shader_module, sizeof(info_log), NULL, info_log);

        char* shader_type_string;
        switch (shader_type) {
            case GL_VERTEX_SHADER: shader_type_string = "vertex"; break;
            case GL_FRAGMENT_SHADER: shader_type_string = "fragment"; break;
            default: shader_type_string = "unknown"; break;
        }

        fprintf(stderr, "[ERROR] [SHADER] Failed to create %s shader module: \n%s\n", shader_type_string, info_log);

        glDeleteShader(shader_module);
        free((void*) shader_source);
        return 0;
    }

    free((void*) shader_source);
    return shader_module;
}

void shader_bind(Shader shader) {
    glUseProgram(shader);
}

void shader_set_mat4_uniform(GLuint uniform_location, mat4 value) {
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, *value);
}

void shader_set_vec3_uniform(GLuint uniform_location, vec3 value) {
    glUniform3fv(uniform_location, 1, value);
}

void shader_set_float_uniform(GLuint uniform_location, GLfloat value) {
    glUniform1f(uniform_location, value);
}

void shader_set_int_uniform(GLuint uniform_location, GLint value) {
    glUniform1i(uniform_location, value);
}

void shader_unbind() {
    glUseProgram(0);
}

void shader_destroy(Shader shader) {
    glDeleteProgram(shader);

    printf("[INFO] [Shader] Destroyed shader!\n");
}
