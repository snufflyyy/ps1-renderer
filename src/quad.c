#include "quad.h"

#include <glad/glad.h>
#include <stddef.h>

#include "vertex.h"

const Vertex vertices[] = {
    { { -0.5f, -0.5f }, { 0.0f, 0.0f } }, // bottom left
    { {  0.5f, -0.5f }, { 1.0f, 0.0f } }, // bottom right
    { {  0.5f,  0.5f }, { 1.0f, 1.0f } }, // top right
    { { -0.5f,  0.5f }, { 0.0f, 1.0f } }, // top left
};
const GLuint indices[] = {
    0, 1, 2,   // first triangle
    0, 2, 3    // second triangle
};

Quad quad_create() {
	Quad quad = {0};

	glGenVertexArrays(1, &quad.vao);
	glBindVertexArray(quad.vao);

	glGenBuffers(1, &quad.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &quad.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return quad;
}

void quad_draw(Quad* quad) {
	glBindVertexArray(quad->vao);
	glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(GLuint)), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void quad_destroy(Quad* quad) {
	glDeleteBuffers(1, &quad->ebo);
	glDeleteBuffers(1, &quad->vbo);
	glDeleteVertexArrays(1, &quad->vao);
}
