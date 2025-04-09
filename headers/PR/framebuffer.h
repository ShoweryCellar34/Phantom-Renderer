#pragma once

#include <glad/gl.h>

typedef struct prTextureData prTextureData;

typedef struct prFramebufferData {
    GladGLContext* context;
    GLuint FBO;
    prTextureData* colorTexture;
    prTextureData* depthTexture;
    prTextureData* stencilTexture;
    prTextureData* depthStencilTexture;
} prFramebufferData;

prFramebufferData* prFramebufferCreate();

void prFramebufferDestroy(prFramebufferData* framebuffer);

void prFramebufferLinkContext(prFramebufferData* framebuffer, GladGLContext* context);

void prFramebufferLinkColorTexture(prFramebufferData* framebuffer, prTextureData* colorTexture);

void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture);

void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture);

void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture);
