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

void prFramebufferBind(prFramebufferData* framebuffer);
void prFramebufferUnbind(GladGLContext* context);
void prFramebufferBindRead(prFramebufferData* framebuffer);
void prFramebufferBindDraw(prFramebufferData* framebuffer);
GLuint prFramebufferGetHandle(prFramebufferData* framebuffer);
GLenum prFramebufferCheckStatus(prFramebufferData* framebuffer);

void prFramebufferLinkColorTexture(prFramebufferData* framebuffer, prTextureData* colorTexture);
void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture);
void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture);
void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture);

void prFramebufferLinkColorTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* colorRBO);
void prFramebufferLinkDepthTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* depthRBO);
void prFramebufferLinkStencilTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* stencilRBO);
void prFramebufferLinkDepthStencilTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* depthStencilRBO);

void prFramebufferTextureAttachment(prFramebufferData* framebuffer, GLenum attachment, GLuint textureHandle, GLint level);
void prFramebufferRenderBufferAttachment(prFramebufferData* framebuffer, GLenum attachment, GLuint rboHandle);

void prFramebufferBlit(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination,
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
    GLbitfield mask, GLenum filter
);

void prFramebufferClearColor(GladGLContext* context, prFramebufferData* framebuffer, GLfloat color[4]);

void prFramebufferClearDepth(GladGLContext* context, prFramebufferData* framebuffer, GLfloat depth);

void prFramebufferClearStencil(GladGLContext* context, prFramebufferData* framebuffer, GLint stencil);

void prFramebufferClearDepthStencil(GladGLContext* context, prFramebufferData* framebuffer, GLfloat depth, GLint stencil);
