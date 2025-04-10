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

void prRenderBufferUpdate(prRenderBufferData* renderBuffer, int type, GLint width, GLint height) {
    if((type != PR_COLOR) & (type != PR_DEPTH) & (type != PR_STENCIL) & (type != PR_DEPTH_STENCIL)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prRenderBufferUpdate: Invalid type for renderbuffer (was %i), using PR_COLOR type", type);
        renderBuffer->type = PR_COLOR;
    } else {
        renderBuffer->type = type;
    }

    renderBuffer->width = width;
    renderBuffer->height = height;

    if(renderBuffer->context && !renderBuffer->RBO) {
        i_prRenderBufferCreateOnGPU(renderBuffer);
    } else if(renderBuffer->context && renderBuffer->width && renderBuffer->height) {
        i_prRenderBufferUpdateOnGPU(renderBuffer);
    }
}
