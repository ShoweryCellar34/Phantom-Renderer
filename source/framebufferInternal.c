#include <PR/framebufferInternal.h>

#include <PR/logger.h>
#include <PR/texture.h>
#include <PR/cubeMap.h>
#include <PR/renderbuffer.h>

void i_prFramebufferCreateOnGPU(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prFramebufferCreateOnGPU: Creating framebuffer object");

    framebuffer->context->CreateFramebuffers(1, &framebuffer->FBO);
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
    for(int i = 0; i < PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS; i++) {
        i_prFramebufferSetAttachment(framebuffer, framebuffer->colorAttachments[i], framebuffer->colorAttachmentsTypes[i], GL_COLOR_ATTACHMENT0 + i);
    }
    i_prFramebufferSetAttachment(framebuffer, framebuffer->depthAttachment, framebuffer->depthAttachmentType, GL_DEPTH_ATTACHMENT);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->stencilAttachment, framebuffer->stencilAttachmentType, GL_STENCIL_ATTACHMENT);
    i_prFramebufferSetAttachment(framebuffer, framebuffer->depthStencilAttachment, framebuffer->depthStencilAttachmentType, GL_DEPTH_STENCIL_ATTACHMENT);
}

void i_prFramebufferSetAttachment(prFramebufferData* framebuffer, void* attachment, unsigned int type, GLenum attachmentPoint) {
    if(attachment) {
        switch(type) {
            case 1:
                if(((prTextureData*)attachment)->TBO) {
                    prTextureData* texture = attachment;
                    framebuffer->context->NamedFramebufferTexture(framebuffer->FBO, attachmentPoint, texture->TBO, 0);
                }
                break;

            case 2:
                if(((prCubeMapData*)attachment)->TBO) {
                    prCubeMapData* cubeMap = attachment;
                    framebuffer->context->NamedFramebufferTexture(framebuffer->FBO, attachmentPoint, cubeMap->TBO, 0);
                }
                break;

            case 3:
                if(((prRenderBufferData*)attachment)->RBO) {
                    prRenderBufferData* RBO = attachment;
                    framebuffer->context->NamedFramebufferRenderbuffer(framebuffer->FBO, attachmentPoint, GL_RENDERBUFFER, RBO->RBO);
                }
                break;
        }
    }
}

void i_prFramebufferBlitOnGPU(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination,
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
    GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
    GLbitfield mask, GLenum filter
) {
    context->BlitNamedFramebuffer(
        (source ? source->FBO : 0), (destination ? destination->FBO : 0),
        srcX0, srcY0, srcX1, srcY1,
        dstX0, dstY0, dstX1, dstY1,
        mask, filter
    );
}
