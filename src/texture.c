#include "texture.h"

#include <glad/glad.h>

Texture texture_create() {
  GLuint texture;
  glGenTextures(1, &texture);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture;
}

inline void texture_bind(Texture texture) {
  glBindTexture(GL_TEXTURE_2D, texture);
}

inline void texture_unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}

inline void texture_destroy(Texture texture) {
  glDeleteTextures(1, &texture);
}
