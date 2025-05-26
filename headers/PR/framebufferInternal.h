#pragma once

#include <PR/framebuffer.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferDestroyOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferUpdateBuffers(prFramebufferData* framebuffer);

void i_prFramebufferSetDataOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferSetAttachment(prFramebufferData* framebuffer, prTextureData* texture, prRenderBufferData* renderbuffer, GLenum attachment);

void i_prFramebufferBlitOnGPU(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination, 
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
    GLbitfield mask, GLenum filter
);

void i_prFramebufferClear(GladGLContext* context, prFramebufferData* framebuffer, unsigned int bits);
