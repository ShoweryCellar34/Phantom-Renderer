#include <PR/framebufferInternal.h>

#include <PR/logger.h>
#include <PR/texture.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prFramebufferCreateOnGPU: Creating framebuffer object");

    framebuffer->context->GenFramebuffers(1, &framebuffer->FBO);
    if(!framebuffer->FBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prFramebufferCreateOnGPU: Failed to create framebuffer object. Aborting operation, nothing was modified");
        return;
    }
}

void i_prFramebufferDestroyOnGPU(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prFramebufferDestroyOnGPU: Destroying framebuffer object");

    framebuffer->context->DeleteFramebuffers(1, &framebuffer->FBO);
}

void i_prFramebufferUpdateBuffers(prFramebufferData* framebuffer) {
    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);

    GLuint TBO = 0;
    if(framebuffer->colorTexture) {
        TBO = framebuffer->colorTexture->TBO;
    }
    framebuffer->context->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);

    TBO = 0;
    if(framebuffer->depthTexture) {
        TBO = framebuffer->depthTexture->TBO;
    }
    framebuffer->context->FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, TBO, 0);

    TBO = 0;
    if(framebuffer->stencilTexture) {
        TBO = framebuffer->stencilTexture->TBO;
    }
    framebuffer->context->FramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, TBO, 0);

    TBO = 0;
    if(framebuffer->depthStencilTexture) {
        TBO = framebuffer->depthStencilTexture->TBO;
    }
    framebuffer->context->FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, TBO, 0);

    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, 0);
}
