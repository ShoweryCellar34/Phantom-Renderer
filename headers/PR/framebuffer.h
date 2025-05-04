#pragma once

#include <glad/gl.h>

typedef struct prTextureData prTextureData;
typedef struct prRenderBufferData prRenderBufferData;

typedef struct prFramebufferData {
    GladGLContext* context;
    GLuint FBO;
    prTextureData* colorTexture;
    prTextureData* depthTexture;
    prTextureData* stencilTexture;
    prTextureData* depthStencilTexture;

    prRenderBufferData* colorRBO;
    prRenderBufferData* depthRBO;
    prRenderBufferData* stencilRBO;
    prRenderBufferData* depthStencilRBO;
} prFramebufferData;

prFramebufferData* prFramebufferCreate();

void prFramebufferDestroy(prFramebufferData* framebuffer);

void prFramebufferLinkContext(prFramebufferData* framebuffer, GladGLContext* context);

void prFramebufferLinkColorTexture(prFramebufferData* framebuffer, prTextureData* colorTexture);
void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture);
void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture);
void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture);

void prFramebufferLinkColorTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* colorRBO);
void prFramebufferLinkDepthTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* depthRBO);
void prFramebufferLinkStencilTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* stencilRBO);
void prFramebufferLinkDepthStencilTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* depthStencilRBO);

void prFramebufferBlit(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination, int srcX0, int srcY0, int srcX1, int srcY1, int dstX0, int dstY0, int dstX1, int dstY1, GLbitfield mask, GLenum filter);
