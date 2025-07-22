#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>

typedef struct prTextureMultisampledData {
    GladGLContext* context;
    GLenum format;
    GLsizei width, height, samples;
    GLuint TBO;
} prTextureMultisampledData;

prTextureMultisampledData* prTextureMultisampledCreate();

void prTextureMultisampledDestroy(prTextureMultisampledData* texture);

void prTextureMultisampledLinkContext(prTextureMultisampledData* texture, GladGLContext* context);

void prTextureMultisampledUpdate(prTextureMultisampledData* texture, GLenum format, GLsizei width, GLsizei height, GLsizei samples);

void prTextureMultisampledBindImage(prTextureMultisampledData* texture, GLuint index, GLint mipmapLevel, GLenum access, GLenum format);

void prTextureMultisampledBindTexture(prTextureMultisampledData* texture, unsigned int unit);
