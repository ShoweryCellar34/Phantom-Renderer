#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>

void prEnableImageFlip();

void prDisableImageFlip();

typedef struct prTextureData {
    GladGLContext* context;
    GLenum format;
    GLint wrappingMode;
    GLint filter;
    GLubyte* textureData;
    GLsizei width, height;
    int channels;
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLinkContext(prTextureData* texture, GladGLContext* context);

void prTextureUpdate(prTextureData* texture, GLenum format, GLint wrappingMode, GLint filter, GLubyte rawTextureData[], size_t rawTextureDataCount, GLsizei width, GLsizei height);

void prTextureBindImage(prTextureData* texture, unsigned int index, unsigned int mipmapLevel, unsigned int access, unsigned int format);

void prTextureBindTexture(prTextureData* texture, unsigned int unit);
