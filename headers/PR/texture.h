#pragma once

#include <glad/gl.h>

typedef struct prWindow prWindow;

typedef struct prTextureData {
    prWindow* window;
    GLubyte* textureData;
    GLint width, height, channels;
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLink(prTextureData* texture, prWindow* window);

void prTextureUpdate(prTextureData* texture, unsigned char rawTextureData[], unsigned int rawTextureDataCount);
