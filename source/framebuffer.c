#include <PR/framebuffer.h>

#include <PR/framebufferInternal.h>

#include <PR/memory.h>
#include <PR/texture.h>
#include <PR/cubeMap.h>
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
    }
}

void prFramebufferBind(prFramebufferData* framebuffer) {
    if(!framebuffer->context || !framebuffer->FBO) {
        return;
    }
    framebuffer->context->BindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);
}

void prFramebufferUnbind(GladGLContext* context) {
    context->BindFramebuffer(GL_FRAMEBUFFER, 0);
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

    return framebuffer->context->CheckNamedFramebufferStatus(framebuffer->FBO, GL_FRAMEBUFFER);
}

void prFramebufferLinkColorTexture(prFramebufferData* framebuffer, prTextureData* colorTexture, unsigned int attachmentPoint) {
    if(colorTexture && framebuffer->context != colorTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    if(attachmentPoint >= PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorTexture: Requested attachment point (%i) too high. Aborting operation, nothing was modified");
        return;
    }

    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferLinkColorTexture: Setting framebuffer color attachment %i (Texture)", attachmentPoint);

    framebuffer->colorAttachments[attachmentPoint] = colorTexture;
    framebuffer->colorAttachmentsTypes[attachmentPoint] = 1;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthTexture(prFramebufferData* framebuffer, prTextureData* depthTexture) {
    if(depthTexture && framebuffer->context != depthTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthAttachment = depthTexture;
    framebuffer->depthAttachmentType = 1;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkStencilTexture(prFramebufferData* framebuffer, prTextureData* stencilTexture) {
    if(stencilTexture && framebuffer->context != stencilTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkStencilTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->stencilAttachment = stencilTexture;
    framebuffer->stencilAttachmentType = 1;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthStencilTexture(prFramebufferData* framebuffer, prTextureData* depthStencilTexture) {
    if(depthStencilTexture && framebuffer->context != depthStencilTexture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthStencilTexture: Texture context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthStencilAttachment = depthStencilTexture;
    framebuffer->depthStencilAttachmentType = 1;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkColorCubeMap(prFramebufferData* framebuffer, prCubeMapData* colorCubeMap, unsigned int attachmentPoint) {
    if(framebuffer->context != colorCubeMap->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorCubeMap: Cube map context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    if(attachmentPoint >= PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorCubeMap: Requested attachment point (%i) too high. Aborting operation, nothing was modified");
        return;
    }

    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferLinkColorCubeMap: Setting framebuffer color attachment %i (Cube Map)", attachmentPoint);

    framebuffer->colorAttachments[attachmentPoint] = colorCubeMap;
    framebuffer->colorAttachmentsTypes[attachmentPoint] = 2;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthCubeMap(prFramebufferData* framebuffer, prCubeMapData* depthCubeMap) {
    if(framebuffer->context != depthCubeMap->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthCubeMap: Cube map context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthAttachment = depthCubeMap;
    framebuffer->depthAttachmentType = 2;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkStencilCubeMap(prFramebufferData* framebuffer, prCubeMapData* stencilCubeMap) {
    if(framebuffer->context != stencilCubeMap->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkStencilCubeMap: Cube map context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->stencilAttachment = stencilCubeMap;
    framebuffer->stencilAttachmentType = 2;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthStencilCubeMap(prFramebufferData* framebuffer, prCubeMapData* depthStencilCubeMap) {
    if(framebuffer->context != depthStencilCubeMap->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthStencilCubeMap: Cube map context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthStencilAttachment = depthStencilCubeMap;
    framebuffer->depthStencilAttachmentType = 2;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkColorRBO(prFramebufferData* framebuffer, prRenderBufferData* colorRBO, unsigned int attachmentPoint) {
    if(framebuffer->context != colorRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    if(attachmentPoint >= PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkColorRBO: Requested attachment point (%i) too high. Aborting operation, nothing was modified");
        return;
    }

    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferLinkColorRBO: Setting framebuffer color attachment %i (RBO)", attachmentPoint);

    framebuffer->colorAttachments[attachmentPoint] = colorRBO;
    framebuffer->colorAttachmentsTypes[attachmentPoint] = 3;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthRBO(prFramebufferData* framebuffer, prRenderBufferData* depthRBO) {
    if(framebuffer->context != depthRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthAttachment = depthRBO;
    framebuffer->depthAttachmentType = 3;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkStencilRBO(prFramebufferData* framebuffer, prRenderBufferData* stencilRBO) {
    if(framebuffer->context != stencilRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkStencilRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->stencilAttachment = stencilRBO;
    framebuffer->stencilAttachmentType = 3;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferLinkDepthStencilRBO(prFramebufferData* framebuffer, prRenderBufferData* depthStencilRBO) {
    if(framebuffer->context != depthStencilRBO->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferLinkDepthStencilRBO: RenderBuffer context does not match framebuffer context. Aborting operation, nothing was modified");
        return;
    }
    framebuffer->depthStencilAttachment = depthStencilRBO;
    framebuffer->depthStencilAttachmentType = 3;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferUnlinkColorAttachment(prFramebufferData* framebuffer, unsigned int attachmentPoint) {
    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferUnlinkColorAttachment: Unlinking color attachment (attachment point: %i)", attachmentPoint);

    framebuffer->colorAttachments[attachmentPoint] = NULL;
    framebuffer->colorAttachmentsTypes[attachmentPoint] = 0;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferUnlinkDepthAttachment(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferUnlinkDepthAttachment: Unlinking depth attachment");

    framebuffer->depthAttachment = NULL;
    framebuffer->depthAttachmentType = 0;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferUnlinkStencilAttachment(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferUnlinkStencilAttachment: Unlinking stencil attachment");

    framebuffer->stencilAttachment = NULL;
    framebuffer->stencilAttachmentType = 0;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferUnlinkDepthStencilAttachment(prFramebufferData* framebuffer) {
    prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prFramebufferUnlinkDepthStencilAttachment: Unlinking depth stencil attachment");

    framebuffer->depthStencilAttachment = NULL;
    framebuffer->depthStencilAttachmentType = 0;

    if(framebuffer->FBO) {
        i_prFramebufferUpdateBuffers(framebuffer);
    }
}

void prFramebufferSetDrawBuffer(prFramebufferData* framebuffer, GLenum buffer) {
    if(!framebuffer->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferSetDrawBuffer: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    framebuffer->context->NamedFramebufferDrawBuffer(framebuffer->FBO, buffer);
}

void prFramebufferDrawBuffers(prFramebufferData* framebuffer, GLsizei count, const GLenum* buffers) {
    if(!framebuffer->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferDrawBuffers: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
        return;
    }
    if(count < 1) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferDrawBuffers: Count cannot be less than zero. Aborting operation, nothing was modified");
        return;
    }

    framebuffer->context->NamedFramebufferDrawBuffers(framebuffer->FBO, count, buffers);
}

void prFramebufferSetReadBuffer(prFramebufferData* framebuffer, GLenum buffer) {
    if(!framebuffer->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferSetReadBuffer: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    framebuffer->context->NamedFramebufferReadBuffer(framebuffer->FBO, buffer);
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

void prFramebufferClearColor(GladGLContext* context, prFramebufferData* framebuffer, unsigned int attachmentIndex, GLfloat color[4]) {
    if(attachmentIndex >= PR_MAX_FRAMEBUFFER_COLOR_ATTACHMENTS) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearColor: Attachment index too hight (%i). Aborting operation, nothing was modified", attachmentIndex);
        return;
    }

    if(framebuffer) {
        if(!framebuffer->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearColor: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(framebuffer->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearColor: Framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!framebuffer->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearColor: Framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }

        context->ClearNamedFramebufferfv(framebuffer->FBO, GL_COLOR, attachmentIndex, color);
    } else {
        context->ClearNamedFramebufferfv(0, GL_COLOR, attachmentIndex, color);
    }
}

void prFramebufferClearDepth(GladGLContext* context, prFramebufferData* framebuffer, GLfloat depth) {
    if(framebuffer) {
        if(!framebuffer->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearDepth: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(framebuffer->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearDepth: Framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!framebuffer->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearDepth: Framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }

        context->ClearNamedFramebufferfv(framebuffer->FBO, GL_DEPTH, 0, &depth);
    } else {
        context->ClearNamedFramebufferfv(0, GL_DEPTH, 0, &depth);
    }
}

void prFramebufferClearStencil(GladGLContext* context, prFramebufferData* framebuffer, GLint stencil) {
    if(framebuffer) {
        if(!framebuffer->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearStencil: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(framebuffer->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearStencil: Framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!framebuffer->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearStencil: Framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }

        context->ClearNamedFramebufferiv(framebuffer->FBO, GL_STENCIL, 0, &stencil);
    } else {
        context->ClearNamedFramebufferiv(0, GL_STENCIL, 0, &stencil);
    }
}

void prFramebufferClearDepthStencil(GladGLContext* context, prFramebufferData* framebuffer, GLfloat depth, GLint stencil) {
    if(framebuffer) {
        if(!framebuffer->context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearDepthStencil: Framebuffer context cannot be NULL. Aborting operation, nothing was modified");
            return;
        }
        if(framebuffer->context != context) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearDepthStencil: Framebuffer context must match context passed to function. Aborting operation, nothing was modified");
            return;
        }
        if(!framebuffer->FBO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prFramebufferClearDepthStencil: Framebuffer is not initialized on GPU. Aborting operation, nothing was modified");
            return;
        }

        context->ClearNamedFramebufferfi(framebuffer->FBO, GL_DEPTH_STENCIL, 0, depth, stencil);
    } else {
        context->ClearNamedFramebufferfi(0, GL_DEPTH_STENCIL, 0, depth, stencil);
    }
}
