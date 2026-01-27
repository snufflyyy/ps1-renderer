#include "text.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <ft2build.h>

#include "cglm/vec2.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

static const GLuint indices[6] = {
    0, 1, 2,
    2, 3, 0
};

FontManager font_manager_create() {
    FontManager font_manager = {0};

	if (FT_Init_FreeType(&font_manager.freetype)) {
        fprintf(stderr, "[ERROR] [FONT MANAGER] Failed to initialize FreeType!\n");
        return font_manager;
	}

	font_manager.shader = shader_create("../shaders/text/text.vert", "../shaders/text/text.frag");

    return font_manager;
}

Font font_create(FontManager* font_manager, u32 size, const char* path) {
    Font font = {0};

	if (FT_New_Face(font_manager->freetype, path, 0, &font.face)) {
        fprintf(stderr, "[ERROR] [FONT MANAGER] Failed load font: %s\n", path);
        return font;
	}

	FT_Set_Pixel_Sizes(font.face, 0, size);

	font.has_kerning = FT_HAS_KERNING(font.face);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	usize characters_length = sizeof(font.characters) / sizeof(Character);
	for (usize i = 0; i < characters_length; i++) {
	    if (FT_Load_Char(font.face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "[ERROR] [FONT] Failed to load glyph: %c!\n", ((i32) i));
			continue;
		}

		glGenTextures(1, &font.characters[i].texture);
		texture_bind(font.characters[i].texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.face->glyph->bitmap.width, font.face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, font.face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texture_unbind();

		glm_vec2_copy((vec2) { font.face->glyph->bitmap.width, font.face->glyph->bitmap.rows }, font.characters[i].size);
		glm_vec2_copy((vec2) { font.face->glyph->bitmap_left, font.face->glyph->bitmap_top }, font.characters[i].bearing);
		font.characters[i].advance = font.face->glyph->advance.x;
	}

	glGenVertexArrays(1, &font.vao);
	glGenBuffers(1, &font.vbo);
	glGenBuffers(1, &font.ebo);

	glBindVertexArray(font.vao);

	glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, font.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	return font;
}

void text_draw(Camera* camera, Font* font, FontManager* font_manager, vec2 position, const char* text) {
    shader_bind(font_manager->shader);
    shader_set_mat4_uniform(glGetUniformLocation(font_manager->shader, "view"), camera->view);
	shader_set_mat4_uniform(glGetUniformLocation(font_manager->shader, "projection"), camera->projection);

    shader_set_vec3_uniform(glGetUniformLocation(font_manager->shader, "textColor"), (vec3) { 0.5f, 0.5f, 0.5f });
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->vao);

    FT_UInt previous_index = 0;

    usize text_length = strlen(text);
    for (usize i = 0; i < text_length; i++) {
        FT_UInt index = FT_Get_Char_Index(font->face, (FT_ULong) text[i]);

        if (font->has_kerning) {
            FT_Vector kerning;
            FT_Get_Kerning(font->face, previous_index, index, FT_KERNING_DEFAULT, &kerning);
            position[0] += (kerning.x >> 6);
        }

        Character* character = &font->characters[text[i]];

        float x = position[0] + character->bearing[0];
        float y = position[1] - character->bearing[1];

        vec2 size;
        glm_vec2_copy(character->size, size);

        GLfloat vertices[4][4] = {
            { x,           y + size[1], 0.0f, 1.0f },
            { x,           y,           0.0f, 0.0f },
            { x + size[0], y,           1.0f, 0.0f },
            { x + size[0], y + size[1], 1.0f, 1.0f }
        };

        texture_bind(character->texture);

        glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        position[0] += (character->advance >> 6);
        previous_index = index;
    }

    glBindVertexArray(0);
    texture_unbind();
    shader_unbind();
}

void text_drawf(Camera* camera, Font* font, FontManager* font_manager, vec2 position, const char* format, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    text_draw(camera, font, font_manager, position, buffer);
}

void font_destroy(Font* font) {
    FT_Done_Face(font->face);

    usize characters_length = sizeof(font->characters) / sizeof(Character);
    for (usize i = 0; i < characters_length; i++) {
        texture_destroy(font->characters[i].texture);
    }

    glDeleteBuffers(1, &font->ebo);
    glDeleteBuffers(1, &font->vbo);
    glDeleteVertexArrays(1, &font->vao);
}

void font_manager_destroy(FontManager* font_manager) {
    shader_destroy(font_manager->shader);
    FT_Done_FreeType(font_manager->freetype);
}
