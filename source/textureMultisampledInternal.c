#include <PR/textureMultisampledInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/logger.h>
#include <PR/textureMultisampled.h>

void i_prTextureMultisampledComputeFormats(prTextureMultisampledData* texture, GLint* internalFormat) {
    switch(texture->format) {
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

        case PR_FORMAT_SRGB:
            *internalFormat = GL_SRGB8;
            break;

        case PR_FORMAT_SRGBA:
            *internalFormat = GL_SRGB8_ALPHA8;
            break;

        case PR_FORMAT_STENCIL:
            *internalFormat = GL_STENCIL_INDEX8;
            break;

        case PR_FORMAT_DEPTH:
            *internalFormat = GL_DEPTH_COMPONENT24;
            break;

        case PR_FORMAT_DEPTH_STENCIL:
            *internalFormat = GL_UNSIGNED_INT_24_8;
            break;

        default:
            *internalFormat = GL_RGB16F;
            break;
    }
}

void i_prTextureMultisampledSetDataOnGPU(prTextureMultisampledData* texture) {
    GLint internalFomrat;
    i_prTextureMultisampledComputeFormats(texture, &internalFomrat);

    texture->context->BindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture->TBO);
    texture->context->TexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, texture->samples, internalFomrat, texture->width, texture->height, false);
    texture->context->BindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void i_prTextureMultisampledCreateOnGPU(prTextureMultisampledData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prTextureMultisampledCreateOnGPU: Creating multisampled texture buffer object. Width: %i Height: %i Samples: %i", texture->width, texture->height, texture->samples);

    texture->context->CreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &texture->TBO);
    if(!texture->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prTextureMultisampledCreateOnGPU: Failed to create multisampled texture buffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prTextureMultisampledSetDataOnGPU(texture);
}

void i_prTextureMultisampledDestroyOnGPU(prTextureMultisampledData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prTextureMultisampledDestroyOnGPU: Destroying multisampled texture buffer object");

    texture->context->DeleteTextures(1, &texture->TBO);
}

void i_prTextureMultisampledUpdateOnGPU(prTextureMultisampledData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prTextureMultisampledUpdateOnGPU: Updating mulitsampled texture buffer object. Width: %i Height: %i Samples: %i", texture->width, texture->height, texture->samples);

    if(!texture->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_ERROR, "i_prTextureMultisampledUpdateOnGPU: Failed to update multisampled texture buffer object, invalid ID. Aborting operation, nothing was modified");
        return;
    }

    i_prTextureMultisampledSetDataOnGPU(texture);
}
