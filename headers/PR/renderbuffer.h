#pragma once

#include <stddef.h>
#include <glad/gl.h>

typedef struct prRenderBufferData {
    GladGLContext* context;
    GLenum format;
    GLsizei width, height, samples;
    GLuint RBO;
} prRenderBufferData;

prRenderBufferData* prRenderBufferCreate();

void prRenderBufferDestroy(prRenderBufferData* renderBuffer);

void prRenderBufferLinkContext(prRenderBufferData* renderBuffer, GladGLContext* context);

void prRenderBufferUpdate(prRenderBufferData* renderBuffer, GLenum format, GLsizei width, GLsizei height, GLsizei samples);
