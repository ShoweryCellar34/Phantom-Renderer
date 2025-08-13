#include <PR/textureMultisampled.h>

#include <PR/defines.h>

#include <PR/textureMultisampledInternal.h>

#include <PR/memory.h>
#include <PR/logger.h>

prTextureMultisampledData* prTextureMultisampledCreate() {
    prTextureMultisampledData* texture = prCalloc(1, sizeof(prTextureMultisampledData));

    return texture;
}

void prTextureMultisampledDestroy(prTextureMultisampledData* texture) {
    if(texture->TBO) {
        i_prTextureMultisampledDestroyOnGPU(texture);
    }

    prFree(texture);
}

void prTextureMultisampledLinkContext(prTextureMultisampledData* texture, GladGLContext* context) {
    if(texture->context && texture->TBO) {
        i_prTextureMultisampledDestroyOnGPU(texture);
    }
    texture->context = context;
    if(texture->context && texture->width && texture->height) {
        i_prTextureMultisampledCreateOnGPU(texture);
    }
}

void prTextureMultisampledUpdate(prTextureMultisampledData* texture, GLenum format, GLsizei width, GLsizei height, GLsizei samples) {
    if(!width || !height) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledUpdate: Width or heiht cannot be 0 (Width: %i Height: %i). Aborting operation, nothing was modified", width, height);
        return;
    }

    if(samples < PR_MIN_SAMPLES) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledUpdate: Provided samples (samples: %i min samples: %i) too low. Aborting operation, nothing was modified", samples, PR_MIN_SAMPLES);
        return;
    }
    if(samples > PR_MAX_SAMPLES) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledUpdate: Provided samples (samples: %i max samples: %i) too high. Aborting operation, nothing was modified", samples, PR_MAX_SAMPLES);
        return;
    }

    if(format == PR_FORMAT_AUTO) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledUpdate: PR_FORMAT_AUTO is not supported by multisampled textures. Aborting operation, nothing was modified");
        return;
    }

    if((format != PR_FORMAT_A) && (format != PR_FORMAT_G) && (format != PR_FORMAT_B) &&
        (format != PR_FORMAT_RGB) && (format != PR_FORMAT_RGBA) &&
        (format != PR_FORMAT_STENCIL) && (format != PR_FORMAT_DEPTH) && (format != PR_FORMAT_DEPTH_STENCIL)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledUpdate: Invalid format for texture (was %i). Aborting operation, nothing was modified", format);
        return;
    }

    texture->format = format;
    texture->width = width;
    texture->height = height;
    texture->samples = samples;

    if(texture->context && !texture->TBO) {
        i_prTextureMultisampledCreateOnGPU(texture);
    } else if(texture->context) {
        i_prTextureMultisampledUpdateOnGPU(texture);
    }
}

void prTextureMultisampledBindImage(prTextureMultisampledData* texture, GLuint index, GLint mipmapLevel, GLenum access, GLenum format) {
    if((access != PR_ACCESS_READ_ONLY) && (access != PR_ACCESS_WRITE_ONLY) && (access != PR_ACCESS_READ_WRITE)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureMultisampledBindImage: Invalid access mode (was %u), using PR_ACCESS_READ_WRITE", access);
        access = PR_ACCESS_READ_WRITE;
    }

    if(!texture->TBO) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledBindImage: Attempt to bind texture that has not been created on GPU. Aborting operation");
        return;
    }

    if(!texture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledBindImage: Attempt to bind texture without OpenGL context. Aborting operation");
        return;
    }

    texture->context->BindImageTexture(index, texture->TBO, mipmapLevel, GL_FALSE, 0, access, format);
}

void prTextureMultisampledBindTexture(prTextureMultisampledData* texture, unsigned int unit) {
    if(!texture->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledBindTexture: Attempt to bind texture without OpenGL context. Aborting operation");
        return;
    }
    if(!texture->TBO) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureMultisampledBindTexture: Attempt to bind texture that has not been created on GPU. Aborting operation");
        return;
    }

    texture->context->ActiveTexture(GL_TEXTURE0 + unit);
    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);
}
