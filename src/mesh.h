#pragma once

#include <glad/glad.h>

#include "vertex.h"
#include "base-types.h"
#include "texture.h"

typedef struct Mesh {
    Vertex* vertices;
    usize vertices_count;

    GLuint* indices;
    usize indices_count;

    Texture diffuse_texture;

    GLuint vao, vbo, ebo;
} Mesh;

Mesh mesh_create(Vertex* vertices, usize vertices_count, GLuint* indices, usize indices_count, Texture diffuse_texture);
void mesh_draw(Mesh* mesh);
void mesh_destroy(Mesh* mesh);
