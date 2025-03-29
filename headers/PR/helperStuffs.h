#pragma once

#include <PR/defines.h>

typedef struct GladGLContext GladGLContext;
typedef struct prTextureData prTextureData;
typedef struct prShaderProgramData prShaderProgramData;

prShaderProgramData* loadDefaultShader(GladGLContext* context);

prTextureData* loadTexture(GladGLContext* context, const char* path);

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color);

prTextureData* makeTextureDefault(GladGLContext* context, size_t scale);
