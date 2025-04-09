#pragma once

#include <PR/framebuffer.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferDestroyOnGPU(prFramebufferData* framebuffer);

void i_prFramebufferUpdateBuffers(prFramebufferData* framebuffer);
