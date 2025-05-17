#pragma once

#include <PR/defines.h>

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>

typedef struct prCubeMapData {
    GladGLContext* context;
    GLenum format[PR_CUBE_MAP_SIDES];
    GLint wrappingMode;
    GLint filter;
    GLubyte* textureData[PR_CUBE_MAP_SIDES];
    GLsizei width[PR_CUBE_MAP_SIDES], height[PR_CUBE_MAP_SIDES];
    int channels[PR_CUBE_MAP_SIDES];
    GLuint TBO;
} prCubeMapData;

prCubeMapData* prCubeMapCreate();

void prCubeMapDestroy(prCubeMapData* cubeMap);

void prCubeMapLinkContext(prCubeMapData* cubeMap, GladGLContext* context);

void prCubeMapUpdateAll(prCubeMapData* cubeMap, GLenum format[PR_CUBE_MAP_SIDES], GLint wrappingMode, GLint filter, GLubyte* rawTextureData[PR_CUBE_MAP_SIDES], size_t rawTextureDataCount[PR_CUBE_MAP_SIDES]);

void prCubeMapUpdate(prCubeMapData* cubeMap, int side, GLenum format, GLint wrappingMode, GLint filter, GLubyte* rawTextureData, size_t rawTextureDataCount);

void prCubeMapBindTexture(prCubeMapData* cubeMap, unsigned int unit);
