#ifndef PR_HELPER_STUFFS_H
#define PR_HELPER_STUFFS_H

#include <cglm/struct.h>
#include <PR/defines.h>

#include <stdbool.h>

typedef struct GladGLContext GladGLContext;
typedef struct prTextureData prTextureData;
typedef struct prCubeMapData prCubeMapData;
typedef struct prShaderData prShaderData;
typedef struct prComputeShaderData prComputeShaderData;

prTextureData* loadTexture(GladGLContext* context, unsigned int minFiltering, unsigned int magFiltering, const char* path);

prTextureData* makeTextureSingleColor(GladGLContext* context, vec4s color);

prTextureData* makeTextureCheckerboard(GladGLContext* context, size_t scale, vec4s color1, vec4s color2);

prCubeMapData* loadCubeMap(GladGLContext* context, unsigned int minFiltering, unsigned int magFiltering, const char* paths[PR_CUBE_MAP_SIDES]);

prCubeMapData* makeCubeMapSingleColors(GladGLContext* context, vec4s color[PR_CUBE_MAP_SIDES]);

prCubeMapData* makeCubeMapCheckerboards(GladGLContext* context, size_t scale, vec4s color1[PR_CUBE_MAP_SIDES], vec4s color2[PR_CUBE_MAP_SIDES]);

prShaderData* loadShader(GladGLContext* context, const char* vertexPath, const char* fragmentPath, const char* geometryPath);

prComputeShaderData* loadComputeShader(GladGLContext* context, const char* path);

#endif // PR_HELPER_STUFFS_H
