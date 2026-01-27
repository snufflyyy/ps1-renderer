#pragma once

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.h"
#include "base-types.h"
#include "shader.h"
#include "camera.h"

typedef struct Character {
    Texture texture;
    vec2 size;
    vec2 bearing;
    u32 advance;
} Character;

typedef struct Font {
    FT_Face face;
    Character characters[128];

    bool has_kerning;

    GLuint vao, vbo, ebo;
} Font;

typedef struct FontManager {
    FT_Library freetype;
    Shader shader;
} FontManager;

FontManager font_manager_create();
Font font_create(FontManager* font_manager, u32 size, const char* path);
void text_draw(Camera* camera, Font* font, FontManager* font_manager, vec2 position, const char* text);
void text_drawf(Camera* camera, Font* font, FontManager* font_manager, vec2 position, const char* format, ...);
void font_destroy(Font* font);
void font_manager_destroy(FontManager* font_manager);
