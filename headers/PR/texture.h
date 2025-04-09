#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>

void prEnableImageFlip();

void prDisableImageFlip();

typedef struct prTextureData {
    GladGLContext* context;
    int type;
    int wrappingMode;
    bool pixelated;
    GLubyte* textureData;
    GLint width, height, channels;
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLinkContext(prTextureData* texture, GladGLContext* context);

void prTextureUpdate(prTextureData* texture, int type, int wrappingMode, GLubyte rawTextureData[], size_t rawTextureDataCount, GLint width, GLint height);

void prTextureSetPixelated(prTextureData* texture, bool pixelated);
