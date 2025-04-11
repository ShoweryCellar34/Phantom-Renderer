#include <PR/framebufferInternal.h>

#include <PR/logger.h>
#include <PR/texture.h>
#include <PR/renderbuffer.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prFramebufferCreateOnGPU: Creating framebuffer object");

    framebuffer->context->GenFramebuffers(1, &framebuffer->FBO);
    if(!framebuffer->FBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prFramebufferCreateOnGPU: Failed to create framebuffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prFramebufferSetDataOnGPU(framebuffer);
}

void i_prFramebufferDestroyOnGPU(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prFramebufferDestroyOnGPU: Destroying framebuffer object");

    framebuffer->context->DeleteFramebuffers(1, &framebuffer->FBO);
}

void i_prFramebufferUpdateBuffers(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prFramebufferUpdateBuffers: Updating framebuffer object");
    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);

    i_prFramebufferSetDataOnGPU(framebuffer);
}

void i_prFramebufferSetDataOnGPU(prFramebufferData* framebuffer) {
    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, 0);

    i_prFramebufferSetAttachment(framebuffer, framebuffer->colorTexture, framebuffer->colorRBO, GL_COLOR_ATTACHMENT0);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->depthTexture, framebuffer->depthRBO, GL_DEPTH_ATTACHMENT);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->stencilTexture, framebuffer->stencilRBO, GL_STENCIL_ATTACHMENT);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->depthStencilTexture, framebuffer->depthStencilRBO, GL_DEPTH_STENCIL_ATTACHMENT);

    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void i_prFramebufferSetAttachment(prFramebufferData* framebuffer, prTextureData* texture, prRenderBufferData* renderbuffer, GLenum attachment) {
    GLuint TBO = 0;
    GLuint RBO = 0;

    if(texture) {
        if(texture->TBO) {
            if(texture->context != framebuffer->context) {
                prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prFramebufferSetAttachment: Texture context does not match framebuffer context, binding ID 0");
            }
            TBO = texture->TBO;
        }
    }
    framebuffer->context->FramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, TBO, 0);

    if(renderbuffer) {
        if(renderbuffer->RBO) {
            if(renderbuffer->context != framebuffer->context) {
                prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prFramebufferSetAttachment: Renderbuffer context does not match framebuffer context, binding ID 0");
            }
            RBO = renderbuffer->RBO;
        }
    }
    framebuffer->context->FramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, RBO);
}
