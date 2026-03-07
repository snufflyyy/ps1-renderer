#include "gfx/mesh.h"
#include "vertex.h"

#include <glad/glad.h>

#include "utils/base-types.h"
#include "texture.h"

Mesh mesh_create(Vertex* vertices, usize vertices_count, GLuint* indices, usize indices_count, Texture diffuse_texture) {
	Mesh mesh = {0};

    mesh.vertices = vertices;
    mesh.vertices_count = vertices_count;

    mesh.indices = indices;
    mesh.indices_count = indices_count;

    mesh.diffuse_texture = diffuse_texture;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    glBufferData(GL_ARRAY_BUFFER, (u32) mesh.vertices_count * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (u32) mesh.indices_count * sizeof(GLuint), &mesh.indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texture_coords));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return mesh;
}

void mesh_draw(Mesh* mesh) {
    glBindVertexArray(mesh->vao);
    texture_bind(GL_TEXTURE0, mesh->diffuse_texture);
    glDrawElements(GL_TRIANGLES, (GLsizei) mesh->indices_count, GL_UNSIGNED_INT, 0);
    texture_unbind();
    glBindVertexArray(0);
}

void mesh_destroy(Mesh* mesh) {
	if (mesh->vertices) { free(mesh->vertices); }
	mesh->vertices_count = 0;
	if (mesh->indices) { free(mesh->indices); }
	mesh->indices_count = 0;

	texture_destroy(mesh->diffuse_texture);

	glDeleteVertexArrays(1, &mesh->vao);
	glDeleteBuffers(1, &mesh->vbo);
	glDeleteBuffers(1, &mesh->ebo);
}
