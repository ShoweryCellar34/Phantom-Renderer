#pragma once

#include <PR/framebuffer.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferDestroyOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferUpdateBuffers(prFramebufferData* framebuffer);

void i_prFramebufferSetDataOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferSetAttachment(prFramebufferData* framebuffer, prTextureData* texture, prRenderBufferData* renderbuffer, GLenum attachment);

void i_prFramebufferBlitOnGPU(prFramebufferData* source, prFramebufferData* destination, 
    int srcX0, int srcY0, int srcX1, int srcY1,
    int dstX0, int dstY0, int dstX1, int dstY1,
    GLbitfield mask, GLenum filter);
