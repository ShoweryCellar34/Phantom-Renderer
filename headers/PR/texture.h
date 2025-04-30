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
    bool pixelated;
    GLubyte* textureData;
    GLint width, height, channels;
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLinkContext(prTextureData* texture, GladGLContext* context);

void prTextureUpdate(prTextureData* texture, int format, int wrappingMode, GLubyte rawTextureData[], size_t rawTextureDataCount, GLint width, GLint height);

void prTextureSetPixelated(prTextureData* texture, bool pixelated);

void prTextureBindImage(prTextureData* texture, unsigned int index, unsigned int mipmapLevel, unsigned int access, unsigned int format);
