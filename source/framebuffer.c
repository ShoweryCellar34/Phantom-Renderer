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

void prFramebufferBind(prFramebufferData* framebuffer) {
    if(!framebuffer || !framebuffer->context) {
        return;
    }
    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);
}

void prFramebufferUnbind(prFramebufferData* framebuffer) {
    if(!framebuffer || !framebuffer->context) {
        return;
    }
    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, 0);
}

void prFramebufferBindRead(prFramebufferData* framebuffer) {
    if(!framebuffer || !framebuffer->context) {
        return;
    }
    framebuffer->context->BindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->FBO);
}

void prFramebufferBindDraw(prFramebufferData* framebuffer) {
    if(!framebuffer || !framebuffer->context) {
        return;
    }
    framebuffer->context->BindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->FBO);
}

GLuint prFramebufferGetHandle(prFramebufferData* framebuffer) {
    if(!framebuffer) {
        return 0;
    }
    return framebuffer->FBO;
}

GLenum prFramebufferCheckStatus(prFramebufferData* framebuffer) {
    if(!framebuffer || !framebuffer->context) {
        return GL_FRAMEBUFFER_UNDEFINED;
    }
    return framebuffer->context->CheckFramebufferStatus(GL_FRAMEBUFFER);
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
    if(depthStencilTexture && framebuffer->context != depthStencilTexture->context) {
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
    if(stencilRBO && framebuffer->context != stencilRBO->context) {
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

void prFramebufferTextureAttachment(prFramebufferData* framebuffer, GLenum attachment, GLuint textureHandle, GLint level) {
    if (!framebuffer || !framebuffer->context) {
        return;
    }
    framebuffer->context->FramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureHandle, level);
}

void prFramebufferRenderBufferAttachment(prFramebufferData* framebuffer, GLenum attachment, GLuint rboHandle) {
    if (!framebuffer || !framebuffer->context) {
        return;
    }
    framebuffer->context->FramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rboHandle);
}

void prFramebufferBlit(GladGLContext* context, prFramebufferData* source, prFramebufferData* destination, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    if(source) {
        if(!source->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferBlit: Source framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(source->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferBlit: Source framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!source->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferBlit: Source framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }
    }
    if(destination) {
        if(!destination->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferBlit: Destination framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(destination->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferBlit: Destination framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!destination->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferBlit: Destination framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }
    }

    i_prFramebufferBlitOnGPU(context, source, destination, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);
}

void prFramebufferClear(GladGLContext* context, prFramebufferData* framebuffer, GLenum bits) {
    if(framebuffer) {
        if(!framebuffer->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClear: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(framebuffer->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClear: Framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!framebuffer->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClear: Framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }
    }

    i_prFramebufferClear(context, framebuffer, bits);
}
