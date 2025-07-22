#pragma once

#include <PR/defines.h>

#include <glad/gl.h>

typedef struct prTextureData prTextureData;
typedef struct prTextureMultisampledData prTextureMultisampledData;
typedef struct prCubeMapData prCubeMapData;
typedef struct prRenderBufferData prRenderBufferData;

typedef struct prFramebufferData {
    GladGLContext* context;
    GLuint FBO;
    void* colorAttachments[PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS];
    void* depthAttachment;
    void* stencilAttachment;
    void* depthStencilAttachment;
    int colorAttachmentsTypes[PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS];
    int depthAttachmentType;
    int stencilAttachmentType;
    int depthStencilAttachmentType;
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

void prFramebufferLinkColorTexture(prFramebufferData* framebuffer, prTextureData* colorTexture, unsigned int attachmentPoint);
void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture);
void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture);
void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture);

void prFramebufferLinkColorTextureMultisampled(prFramebufferData* framebuffer, prTextureMultisampledData* colorTexture, unsigned int attachmentPoint);
void prFramebufferLinkDepthTextureMultisampled(prFramebufferData* framebuffer, prTextureMultisampledData* depthTexture);
void prFramebufferLinkStencilTextureMultisampled(prFramebufferData* framebuffer, prTextureMultisampledData* stencilTexture);
void prFramebufferLinkDepthStencilTextureMultisampled(prFramebufferData* framebuffer, prTextureMultisampledData* depthStencilTexture);

void prFramebufferLinkColorCubeMap(prFramebufferData* framebuffer, prCubeMapData* colorCubeMap, unsigned int attachmentPoint);
void prFramebufferLinkDepthCubeMap(prFramebufferData* framebuffer, prCubeMapData* depthCubeMap);
void prFramebufferLinkStencilCubeMap(prFramebufferData* framebuffer, prCubeMapData* stencilCubeMap);
void prFramebufferLinkDepthStencilCubeMap(prFramebufferData* framebuffer, prCubeMapData* depthStencilCubeMap);

void prFramebufferLinkColorRBO(prFramebufferData* framebuffer, prRenderBufferData* colorRBO, unsigned int attachmentPoint);
void prFramebufferLinkDepthRBO(prFramebufferData* framebuffer, prRenderBufferData* depthRBO);
void prFramebufferLinkStencilRBO(prFramebufferData* framebuffer, prRenderBufferData* stencilRBO);
void prFramebufferLinkDepthStencilRBO(prFramebufferData* framebuffer, prRenderBufferData* depthStencilRBO);

void prFramebufferUnlinkColorAttachment(prFramebufferData* framebuffer, unsigned int attachmentPoint);
void prFramebufferUnlinkDepthAttachment(prFramebufferData* framebuffer);
void prFramebufferUnlinkStencilAttachment(prFramebufferData* framebuffer);
void prFramebufferUnlinkDepthStencilAttachment(prFramebufferData* framebuffer);

void prFramebufferSetDrawBuffer(prFramebufferData* framebuffer, GLenum buffer);
void prFramebufferSetReadBuffer(prFramebufferData* framebuffer, GLenum buffer);

void prFramebufferDrawBuffers(prFramebufferData* framebuffer, GLsizei count, const GLenum* buffers);

void prFramebufferBlit(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination,
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
    GLbitfield mask, GLenum filter
);

void prFramebufferClearColor(GladGLContext* context, prFramebufferData* framebuffer, unsigned int attachmentIndex, GLfloat color[4]);

void prFramebufferClearDepth(GladGLContext* context, prFramebufferData* framebuffer, GLfloat depth);

void prFramebufferClearStencil(GladGLContext* context, prFramebufferData* framebuffer, GLint stencil);

void prFramebufferClearDepthStencil(GladGLContext* context, prFramebufferData* framebuffer, GLfloat depth, GLint stencil);
