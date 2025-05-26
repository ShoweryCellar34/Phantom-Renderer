#include <PR/renderbufferInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/logger.h>
#include <PR/renderbuffer.h>

void i_prRenderbufferComputeFormats(prRenderBufferData* renderBuffer, GLenum* internalFormat) {
    switch(renderBuffer->format) {
        case PR_FORMAT_R:
            *internalFormat = GL_RED;
            break;

        case PR_FORMAT_G:
            *internalFormat = GL_GREEN;
            break;

        case PR_FORMAT_B:
            *internalFormat = GL_BLUE;
            break;

        case PR_FORMAT_A:
            *internalFormat = GL_ALPHA;
            break;

        case PR_FORMAT_RGB:
            *internalFormat = GL_RGB16F;
            break;

        case PR_FORMAT_RGBA:
            *internalFormat = GL_RGBA32F;
            break;

        case PR_FORMAT_STENCIL:
            *internalFormat = GL_STENCIL_INDEX8;
            break;

        case PR_FORMAT_DEPTH:
            *internalFormat = GL_DEPTH_COMPONENT24;
            break;

        case PR_FORMAT_DEPTH_STENCIL:
            *internalFormat = GL_DEPTH24_STENCIL8;
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
