#pragma once

#include <PR/defines.h>

typedef struct GladGLContext GladGLContext;
typedef struct prShaderData prShaderData;
typedef struct prTextureData prTextureData;
typedef struct prCubeMapData prCubeMapData;

prTextureData* loadTexture(GladGLContext* context, unsigned int filtering, const char* path);

prCubeMapData* loadCubeMap(GladGLContext* context, unsigned int filtering, const char* paths[PR_CUBE_MAP_SIDES]);

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color);

prTextureData* makeTextureCheckerboard(GladGLContext* context, size_t scale, float* color1, float* color2);
