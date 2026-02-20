#pragma once

#include <glad/glad.h>

#include "vertex.h"
#include "base-types.h"
#include "texture.h"

typedef struct Mesh {
    Vertex* vertices;
    usize number_of_vertices;

    GLuint* indices;
    usize number_of_indices;

    Texture diffuse_texture;

    GLuint vao, vbo, ebo;
} Mesh;

Mesh mesh_create(Vertex* vertices, usize number_of_vertices, GLuint* indices, usize number_of_indices, Texture diffuse_texture);
void mesh_draw(Mesh* mesh);
void mesh_destroy(Mesh* mesh);
