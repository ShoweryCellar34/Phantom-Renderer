#pragma once

#include <PR/framebuffer.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferDestroyOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferUpdateBuffers(prFramebufferData* framebuffer);

void i_prFramebufferSetDataOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferSetAttachment(prFramebufferData* framebuffer, prTextureData* texture, prRenderBufferData* renderbuffer, GLenum attachment);
