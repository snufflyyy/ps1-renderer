#include "texture.h"

#include <stdbool.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture texture_create(const char* image_path) {
    Texture texture = {0};

    int image_width, image_height, image_channels_count;
    stbi_uc* image = stbi_load(image_path, &image_width, &image_height, &image_channels_count, 4);
    if (!image) {
        fprintf(stderr, "[ERROR] [Texture] Failed to load image!\n");
        return texture;
    }

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image);

    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void texture_bind(GLenum active_texture, Texture texture) {
    glActiveTexture(active_texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void texture_unbind(void) {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_destroy(Texture texture) {
    glDeleteTextures(1, &texture);
}
