#include <PR/renderbuffer.h>

#include <PR/defines.h>

#include <PR/renderbufferInternal.h>

#include <PR/memory.h>
#include <PR/logger.h>

prRenderBufferData* prRenderBufferCreate() {
    prRenderBufferData* renderBuffer = prCalloc(1, sizeof(prRenderBufferData));

    return renderBuffer;
}

void prRenderBufferDestroy(prRenderBufferData* renderBuffer) {
    if(renderBuffer->RBO) {
        i_prRenderBufferDestroyOnGPU(renderBuffer);
    }

    prFree(renderBuffer);
}

void prRenderBufferLinkContext(prRenderBufferData* renderBuffer, GladGLContext* context) {
    if(renderBuffer->context && renderBuffer->RBO) {
        i_prRenderBufferDestroyOnGPU(renderBuffer);
    }
    renderBuffer->context = context;
    if(renderBuffer->context && renderBuffer->width && renderBuffer->height) {
        i_prRenderBufferCreateOnGPU(renderBuffer);
    }
}

void prRenderBufferUpdate(prRenderBufferData* renderBuffer, GLenum format, GLsizei width, GLsizei height, GLsizei samples) {
    if(width <= 0 || height <= 0) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prRenderBufferUpdate: Invalid dimensions (width: %i, height: %i). Aborting operation, nothing was modified", width, height);
        return;
    }
    if(samples >= 4) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_INFO, "prRenderBufferUpdate: Using multisamples (samples: %i)", samples);
    }
    if((format != PR_FORMAT_A) && (format != PR_FORMAT_G) && (format != PR_FORMAT_B) &&
        (format != PR_FORMAT_RGB) && (format != PR_FORMAT_RGBA) &&
        (format != PR_FORMAT_STENCIL) && (format != PR_FORMAT_DEPTH) && (format != PR_FORMAT_DEPTH_STENCIL)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prRenderBufferUpdate: Invalid format for renderbuffer (was %i), using PR_FORMAT_RGB type", format);
        renderBuffer->format = PR_FORMAT_RGB;
    } else {
        renderBuffer->format = format;
    }

    renderBuffer->width = width;
    renderBuffer->height = height;
    renderBuffer->samples = samples;

    if(renderBuffer->context && !renderBuffer->RBO) {
        i_prRenderBufferCreateOnGPU(renderBuffer);
    } else if(renderBuffer->context && renderBuffer->width && renderBuffer->height) {
        i_prRenderBufferUpdateOnGPU(renderBuffer);
    }
}
