#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <glad/gl.h>

typedef struct prTextureData {
    GladGLContext* context;
    GLenum format;
    GLint wrappingMode;
    GLint filter;
    GLubyte* textureData;
    GLsizei width, height;
    int channels;
    GLfloat borderColor[4];
    GLuint TBO;
} prTextureData;

prTextureData* prTextureCreate();

void prTextureDestroy(prTextureData* texture);

void prTextureLinkContext(prTextureData* texture, GladGLContext* context);

void prTextureUpdate(prTextureData* texture, GLenum format, GLint wrappingMode, GLint filter, GLubyte* rawTextureData, size_t rawTextureDataCount, GLsizei width, GLsizei height);

void prTextureBorderColor(prTextureData* texture, GLfloat borderColor[4]);

void prTextureBindImage(prTextureData* texture, GLuint index, GLint mipmapLevel, GLenum access, GLenum format);

void prTextureBindTexture(prTextureData* texture, unsigned int unit);
