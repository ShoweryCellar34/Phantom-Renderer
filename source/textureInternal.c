#include <PR/textureInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/logger.h>
#include <PR/texture.h>

void i_prTextureComputeFormats(prTextureData* texture, GLenum* format, GLint* internalFormat) {
    switch(texture->format) {
        case PR_FORMAT_R:
            *format = GL_RED;
            *internalFormat = GL_RED;
            break;

        case PR_FORMAT_G:
            *format = GL_GREEN;
            *internalFormat = GL_GREEN;
            break;

        case PR_FORMAT_B:
            *format = GL_BLUE;
            *internalFormat = GL_BLUE;
            break;

        case PR_FORMAT_A:
            *format = GL_ALPHA;
            *internalFormat = GL_ALPHA;
            break;

        case PR_FORMAT_RGB:
            *format = GL_RGB;
            *internalFormat = GL_RGB16F;
            break;

        case PR_FORMAT_RGBA:
            *format = GL_RGBA;
            *internalFormat = GL_RGBA32F;
            break;

        case PR_FORMAT_SRGB:
            *format = GL_RGB;
            *internalFormat = GL_SRGB8;
            break;

        case PR_FORMAT_SRGBA:
            *format = GL_RGBA;
            *internalFormat = GL_SRGB8_ALPHA8;
            break;

        case PR_FORMAT_STENCIL:
            *format = GL_STENCIL_INDEX;
            *internalFormat = GL_STENCIL_INDEX8;
            break;

        case PR_FORMAT_DEPTH:
            *format = GL_DEPTH_COMPONENT;
            *internalFormat = GL_DEPTH_COMPONENT24;
            break;

        case PR_FORMAT_DEPTH_STENCIL:
            *format = GL_DEPTH24_STENCIL8;
            *internalFormat = GL_UNSIGNED_INT_24_8;
            break;

        default:
            *format = GL_RGB;
            *internalFormat = GL_RGB16F;
            break;
    }
}

void i_prTextureSetDataOnGPU(prTextureData* texture) {
    GLenum format;
    GLint internalFomrat;
    i_prTextureComputeFormats(texture, &format, &internalFomrat);

    texture->context->TextureParameteri(texture->TBO, GL_TEXTURE_WRAP_S, texture->wrappingMode);
    texture->context->TextureParameteri(texture->TBO, GL_TEXTURE_WRAP_T, texture->wrappingMode);
    texture->context->TextureParameteri(texture->TBO, GL_TEXTURE_MIN_FILTER, texture->filter);
    texture->context->TextureParameteri(texture->TBO, GL_TEXTURE_MAG_FILTER, texture->filter);

    texture->context->PixelStorei(GL_UNPACK_ALIGNMENT, 1);

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);
    texture->context->TexImage2D(GL_TEXTURE_2D, 0, internalFomrat, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->textureData);
    texture->context->BindTexture(GL_TEXTURE_2D, 0);

    texture->context->GenerateTextureMipmap(texture->TBO);
}

void i_prTextureCreateOnGPU(prTextureData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prTextureCreateOnGPU: Creating texture buffer object. Width: %i Height: %i Channels: %i", texture->width, texture->height, texture->channels);

    texture->context->CreateTextures(GL_TEXTURE_2D, 1, &texture->TBO);
    if(!texture->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prTextureCreateOnGPU: Failed to create texture buffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prTextureSetDataOnGPU(texture);
}

void i_prTextureDestroyOnGPU(prTextureData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prTextureDestroyOnGPU: Destroying texture buffer object");

    texture->context->DeleteTextures(1, &texture->TBO);
}

void i_prTextureUpdateOnGPU(prTextureData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prTextureUpdateOnGPU: Updating texture buffer object. Width: %i Height: %i Channels: %i", texture->width, texture->height, texture->channels);

    if(!texture->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prTextureUpdateOnGPU: Failed to update texture buffer object, invalid ID. Aborting operation, nothing was modified");
        return;
    }

    i_prTextureSetDataOnGPU(texture);
}
