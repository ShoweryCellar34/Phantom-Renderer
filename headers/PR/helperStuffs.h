#pragma once

#include <PR/defines.h>

typedef struct GladGLContext GladGLContext;
typedef struct prShaderProgramData prShaderProgramData;
typedef struct prMaterialData prMaterialData;
typedef struct prTextureData prTextureData;

prShaderProgramData* loadDefaultShader(GladGLContext* context);

prMaterialData* makeMaterialOneTexture(prTextureData* texture);

prTextureData* loadTexture(GladGLContext* context, const char* path);

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color);

prTextureData* makeTextureCheckerboard(GladGLContext* context, size_t scale, float* color1, float* color2);
