#pragma once

#include <PR/defines.h>

typedef struct GladGLContext GladGLContext;
typedef struct prShaderData prShaderData;
typedef struct prTextureData prTextureData;

prTextureData* loadTexture(GladGLContext* context, unsigned int filtering, const char* path);

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color);

prTextureData* makeTextureCheckerboard(GladGLContext* context, size_t scale, float* color1, float* color2);
