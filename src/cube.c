#include "cube.h"

#include <string.h>

#include <glad/glad.h>

#include "gfx/mesh.h"
#include "gfx/texture.h"
#include "gfx/vertex.h"

Vertex cube_vertices[24] = {
    { {  0.5f, -0.5f, -0.5f }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, -0.5f }, {  1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { {  0.5f,  0.5f,  0.5f }, {  1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
    { {  0.5f, -0.5f,  0.5f }, {  1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },

    { { -0.5f, -0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -0.5f,  0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
    { { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },

    { { -0.5f,  0.5f, -0.5f }, {  0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f,  0.5f,  0.5f }, {  0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
    { {  0.5f,  0.5f,  0.5f }, {  0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
    { {  0.5f,  0.5f, -0.5f }, {  0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },

    { { -0.5f, -0.5f,  0.5f }, {  0.0f,-1.0f, 0.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f, -0.5f }, {  0.0f,-1.0f, 0.0f }, { 1.0f, 0.0f } },
    { {  0.5f, -0.5f, -0.5f }, {  0.0f,-1.0f, 0.0f }, { 1.0f, 1.0f } },
    { {  0.5f, -0.5f,  0.5f }, {  0.0f,-1.0f, 0.0f }, { 0.0f, 1.0f } },

    { { -0.5f, -0.5f,  0.5f }, {  0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f,  0.5f }, {  0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
    { {  0.5f,  0.5f,  0.5f }, {  0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
    { { -0.5f,  0.5f,  0.5f }, {  0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },

    { {  0.5f, -0.5f, -0.5f }, {  0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
    { { -0.5f, -0.5f, -0.5f }, {  0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f } },
    { { -0.5f,  0.5f, -0.5f }, {  0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f } },
    { {  0.5f,  0.5f, -0.5f }, {  0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f } },
};

static GLuint cube_indices[36] = {
    0,  1,  2,   2,  3,  0,    // +X
    4,  5,  6,   6,  7,  4,    // -X
    8,  9, 10,  10, 11,  8,    // +Y
   12, 13, 14,  14, 15, 12,    // -Y
   16, 17, 18,  18, 19, 16,    // +Z
   20, 21, 22,  22, 23, 20     // -Z
};

Cube cube_create(const char* texture_path) {
    Cube cube = {0};

    Vertex* vertices = (Vertex*) malloc(sizeof(cube_vertices));
    memcpy(vertices, cube_vertices, sizeof(cube_vertices));

    GLuint* indices = (GLuint*) malloc(sizeof(cube_indices));
    memcpy(indices, cube_indices, sizeof(cube_indices));

    cube.mesh = mesh_create(vertices, (sizeof(cube_vertices) / sizeof(Vertex)), indices, (sizeof(cube_indices) / sizeof(GLuint)), texture_create(texture_path));
    glm_mat4_identity(cube.transform);

    return cube;
}

void cube_update(Cube* cube) {}

void cube_draw(Cube* cube, Shader shader) {
    shader_set_mat4_uniform(glGetUniformLocation(shader, "model"), cube->transform);
    mesh_draw(&cube->mesh);
}

void cube_destroy(Cube* cube) {
    mesh_destroy(&cube->mesh);
}
