#pragma once

#include <PR/defines.h>

typedef struct GladGLContext GladGLContext;
typedef struct prShaderData prShaderData;
typedef struct prTextureData prTextureData;
typedef struct prCubeMapData prCubeMapData;

prTextureData* loadTexture(GladGLContext* context, unsigned int filtering, const char* path);

prTextureData* makeTextureSingleColor(GladGLContext* context, float color[4]);

prTextureData* makeTextureCheckerboard(GladGLContext* context, size_t scale, float* color1, float* color2);

prCubeMapData* loadCubeMap(GladGLContext* context, unsigned int filtering, const char* paths[PR_CUBE_MAP_SIDES]);

prCubeMapData* makeCubeMapSingleColors(GladGLContext* context, float color[PR_CUBE_MAP_SIDES][4]);

prCubeMapData* makeCubeMapCheckerboards(GladGLContext* context, size_t scale, float color1[PR_CUBE_MAP_SIDES][4], float color2[PR_CUBE_MAP_SIDES][4]);
