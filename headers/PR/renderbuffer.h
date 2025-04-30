#pragma once

#include <stddef.h>
#include <glad/gl.h>

typedef struct prRenderBufferData {
    GladGLContext* context;
    int format;
    GLint width, height;
    GLuint RBO;
} prRenderBufferData;

prRenderBufferData* prRenderBufferCreate();

void prRenderBufferDestroy(prRenderBufferData* renderBuffer);

void prRenderBufferLinkContext(prRenderBufferData* renderBuffer, GladGLContext* context);

void prRenderBufferUpdate(prRenderBufferData* renderBuffer, int format, GLint width, GLint height);
