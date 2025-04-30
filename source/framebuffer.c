#include <PR/framebuffer.h>

#include <PR/framebufferInternal.h>

#include <PR/memory.h>
#include <PR/texture.h>
#include <PR/renderbuffer.h>
#include <PR/logger.h>

prFramebufferData* prFramebufferCreate() {
    prFramebufferData* framebuffer = prCalloc(1, sizeof(prFramebufferData));

    return framebuffer;
}

void prFramebufferDestroy(prFramebufferData* framebuffer) {
    if(framebuffer->FBO) {
        i_prFramebufferDestroyOnGPU(framebuffer);
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
    if(colorTexture && framebuffer->context != colorTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->colorTexture = colorTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture) {
    if(depthTexture && framebuffer->context != depthTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthTexture = depthTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture) {
    if(stencilTexture && framebuffer->context != stencilTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkStencilTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->stencilTexture = stencilTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture) {
    if (depthStencilTexture && framebuffer->context != depthStencilTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthStencilTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthStencilTexture = depthStencilTexture;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkColorTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* colorRBO) {
    if(colorRBO && framebuffer->context != colorRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorTextureRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->colorRBO = colorRBO;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* depthRBO) {
    if(depthRBO && framebuffer->context != depthRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthTextureRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthRBO = depthRBO;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkStencilTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* stencilRBO) {
    if (stencilRBO && framebuffer->context != stencilRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkStencilTextureRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->stencilRBO = stencilRBO;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthStencilTextureRBO(prFramebufferData* framebuffer, prRenderBufferData* depthStencilRBO) {
    if(depthStencilRBO && framebuffer->context != depthStencilRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthStencilTextureRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthStencilRBO = depthStencilRBO;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}
