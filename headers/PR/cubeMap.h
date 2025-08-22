#pragma once

#include <PR/defines.h>

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>
#include <cglm/struct.h>

typedef struct prCubeMapData {
    GladGLContext* context;
    GLenum format[PR_CUBE_MAP_SIDES];
    GLint wrappingMode;
    GLint minFilter;
    GLint magFilter;
    bool generateMipmaps;
    GLubyte* textureData[PR_CUBE_MAP_SIDES];
    GLfloat* textureHDRData[PR_CUBE_MAP_SIDES];
    bool HDR[PR_CUBE_MAP_SIDES];
    GLsizei width[PR_CUBE_MAP_SIDES], height[PR_CUBE_MAP_SIDES];
    int channels[PR_CUBE_MAP_SIDES];
    vec4s borderColor;
    GLuint TBO;
} prCubeMapData;

prCubeMapData* prCubeMapCreate();

void prCubeMapDestroy(prCubeMapData* cubeMap);

void prCubeMapLinkContext(prCubeMapData* cubeMap, GladGLContext* context);

void prCubeMapUpdateAll(prCubeMapData* cubeMap, GLenum format[PR_CUBE_MAP_SIDES], GLint wrappingMode, GLint minFilter, GLint magFilter, bool generateMipmaps, GLubyte* rawTextureData[PR_CUBE_MAP_SIDES], size_t rawTextureDataCount[PR_CUBE_MAP_SIDES], GLsizei width[PR_CUBE_MAP_SIDES], GLsizei height[PR_CUBE_MAP_SIDES]);

void prCubeMapUpdate(prCubeMapData* cubeMap, int side, GLenum format, GLint wrappingMode, GLint minFilter, GLint magFilter, bool generateMipmaps, GLubyte* rawTextureData, size_t rawTextureDataCount, GLsizei width, GLsizei height);

void prCubeMapBorderColor(prCubeMapData* cubeMap, vec4s borderColor);

void prCubeMapBindTexture(prCubeMapData* cubeMap, GLuint unit);
