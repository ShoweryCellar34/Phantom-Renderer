#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>

void prEnableImageFlip();

void prDisableImageFlip();

typedef struct prTextureData {
    GladGLContext* context;
    int format;
    int wrappingMode;
    int filter;
    GLubyte* textureData;
    GLint width, height, channels;
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLinkContext(prTextureData* texture, GladGLContext* context);

void prTextureUpdate(prTextureData* texture, int format, int filter, int wrappingMode, GLubyte rawTextureData[], size_t rawTextureDataCount, GLint width, GLint height);

void prTextureBindImage(prTextureData* texture, unsigned int index, unsigned int mipmapLevel, unsigned int access, unsigned int format);
