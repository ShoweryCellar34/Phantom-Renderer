#include <PR/framebuffer.h>

#include <PR/framebufferInternal.h>

#include <PR/memory.h>

prFramebufferData* prFramebufferCreate() {
    prFramebufferData* framebuffer = prCalloc(1, sizeof(prFramebufferData));

    return framebuffer;
}

void prFramebufferDestroy(prFramebufferData* framebuffer) {
    if(framebuffer->FBO) {
        i_prFramebufferCreateOnGPU(framebuffer);
    }

    prFree(framebuffer);
}

void prFramebufferLinkContext(prFramebufferData* framebuffer, GladGLContext* context) {
    if(framebuffer->context && framebuffer->FBO) {
        i_prFramebufferDestroyOnGPU(framebuffer);
    }
    framebuffer->context = context;
    if(framebuffer->context && !framebuffer->FBO) {
        i_prFramebufferCreateOnGPU(framebuffer);
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkColorTexture(prFramebufferData* framebuffer, prTextureData* colorTexture) {
    framebuffer->colorTexture = colorTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture) {
    framebuffer->depthTexture = depthTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture) {
    framebuffer->stencilTexture = stencilTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture) {
    framebuffer->depthStencilTexture = depthStencilTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}
