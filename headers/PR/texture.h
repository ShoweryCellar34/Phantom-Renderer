#pragma once

#include <glad/gl.h>

void prEnableImageFlip();

void prDisableImageFlip();

void prDisableBlending(GladGLContext* context);

typedef struct prTextureData {
    GladGLContext* context;
    GLubyte* textureData;
    GLint width, height, channels;
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLinkContext(prTextureData* texture, GladGLContext* context);

void prTextureUpdate(prTextureData* texture, unsigned char rawTextureData[], unsigned int rawTextureDataCount);
