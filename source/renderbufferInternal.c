#include <PR/renderbufferInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/logger.h>
#include <PR/renderbuffer.h>

void i_prRenderbufferComputeFormats(prRenderBufferData* texture, GLenum* internalFormat) {
    switch(texture->type) {
        case PR_COLOR:
            *internalFormat = GL_RGBA;
            break;

        case PR_DEPTH:
            *internalFormat = GL_DEPTH_COMPONENT;
            break;

        case PR_STENCIL:
            *internalFormat = GL_STENCIL_INDEX;
            break;

        case PR_DEPTH_STENCIL:
            *internalFormat = GL_DEPTH24_STENCIL8;
            break;

        default:
            *internalFormat = 0;
            break;
    }
}

void i_prRenderBufferCreateOnGPU(prRenderBufferData* renderBuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prRenderBufferCreateOnGPU: Creating renderbuffer buffer object. Width: %i Height: %i", renderBuffer->width, renderBuffer->height);

    renderBuffer->context->GenRenderbuffers(1, &renderBuffer->RBO);
    if(!renderBuffer->RBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prRenderBufferCreateOnGPU: Failed to create renderbuffer buffer object. Aborting operation, nothing was modified");
        return;
    }

    GLenum internalFomrat;
    i_prRenderbufferComputeFormats(renderBuffer, &internalFomrat);

    renderBuffer->context->BindRenderbuffer(GL_RENDERBUFFER, renderBuffer->RBO);

    renderBuffer->context->RenderbufferStorage(GL_RENDERBUFFER, internalFomrat, renderBuffer->width, renderBuffer->height);

    renderBuffer->context->BindRenderbuffer(GL_RENDERBUFFER, 0);
}

void i_prRenderBufferDestroyOnGPU(prRenderBufferData* renderBuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prRenderBufferDestroyOnGPU: Destroying renderbuffer buffer object");

    renderBuffer->context->DeleteRenderbuffers(1, &renderBuffer->RBO);
}

void i_prRenderBufferUpdateOnGPU(prRenderBufferData* renderBuffer) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prRenderBufferUpdateOnGPU: Updating renderbuffer buffer object. Width: %i Height: %i", renderBuffer->width, renderBuffer->height);

    if(!renderBuffer->RBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prRenderBufferUpdateOnGPU: Failed to update renderbuffer buffer object, invalid ID. Aborting operation, nothing was modified");
        return;
    }

    int internalFomrat;
    i_prRenderbufferComputeFormats(renderBuffer, &internalFomrat);

    renderBuffer->context->BindRenderbuffer(GL_RENDERBUFFER, renderBuffer->RBO);

    renderBuffer->context->RenderbufferStorage(GL_RENDERBUFFER, internalFomrat, renderBuffer->width, renderBuffer->height);

    renderBuffer->context->BindRenderbuffer(GL_RENDERBUFFER, 0);
}
