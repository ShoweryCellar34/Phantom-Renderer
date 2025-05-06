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

    i_prFramebufferSetDataOnGPU(framebuffer);
}

void i_prFramebufferSetDataOnGPU(prFramebufferData* framebuffer) {
    GLint previousReadFramebuffer, previousDrawFramebuffer;
    
    framebuffer->context->GetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previousReadFramebuffer);
    framebuffer->context->GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawFramebuffer);

    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);

    i_prFramebufferSetAttachment(framebuffer, framebuffer->colorTexture, framebuffer->colorRBO, GL_COLOR_ATTACHMENT0);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->depthTexture, framebuffer->depthRBO, GL_DEPTH_ATTACHMENT);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->stencilTexture, framebuffer->stencilRBO, GL_STENCIL_ATTACHMENT);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->depthStencilTexture, framebuffer->depthStencilRBO, GL_DEPTH_STENCIL_ATTACHMENT);

    framebuffer->context->BindFramebuffer(GL_READ_FRAMEBUFFER, previousReadFramebuffer);
    framebuffer->context->BindFramebuffer(GL_DRAW_FRAMEBUFFER, previousDrawFramebuffer);
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

void i_prFramebufferBlitOnGPU(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination,
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
    GLbitfield mask, GLenum filter
) {
    GLint previousReadFramebuffer, previousDrawFramebuffer;
    
    context->GetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previousReadFramebuffer);
    context->GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawFramebuffer);

    context->BindFramebuffer(GL_READ_FRAMEBUFFER, (source ? source->FBO : 0));
    context->BindFramebuffer(GL_DRAW_FRAMEBUFFER, (destination ? destination->FBO : 0));

    context->BlitFramebuffer(
        srcX0, srcY0, srcX1, srcY1,
        dstX0, dstY0, dstX1, dstY1,
        mask, filter
    );

    context->BindFramebuffer(GL_READ_FRAMEBUFFER, previousReadFramebuffer);
    context->BindFramebuffer(GL_DRAW_FRAMEBUFFER, previousDrawFramebuffer);
}

void i_prFramebufferClear(GladGLContext* context, prFramebufferData* framebuffer, unsigned int bits) {
    GLint previousReadFramebuffer, previousDrawFramebuffer;
    
    context->GetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &previousReadFramebuffer);
    context->GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previousDrawFramebuffer);

    context->BindFramebuffer(GL_FRAMEBUFFER, (framebuffer ? framebuffer->FBO : 0));

    context->Clear(bits);

    context->BindFramebuffer(GL_READ_FRAMEBUFFER, previousReadFramebuffer);
    context->BindFramebuffer(GL_DRAW_FRAMEBUFFER, previousDrawFramebuffer);
}
