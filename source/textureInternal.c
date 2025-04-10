#include <PR/textureInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/logger.h>
#include <PR/texture.h>

void i_prTextureComputeFormats(prTextureData* texture, GLenum* format, GLenum* internalFormat) {
    switch(texture->type) {
        case PR_COLOR:
            switch(texture->channels) {
                case 3:
                    *format = GL_RGB;
                    break;
        
                case 4:
                    *format = GL_RGBA;
                    break;

                default:
                    *format = 0;
                    break;
            }
            *internalFormat = GL_RGBA;
            break;

        case PR_DEPTH:
            *format = GL_DEPTH_COMPONENT;
            *internalFormat = GL_DEPTH_COMPONENT;
            break;

        case PR_STENCIL:
            *format = GL_STENCIL_INDEX;
            *internalFormat = GL_STENCIL_INDEX;
            break;

        case PR_DEPTH_STENCIL:
            *format = GL_DEPTH24_STENCIL8;
            *internalFormat = GL_UNSIGNED_INT_24_8;
            break;

        default:
            *format = 0;
            *internalFormat = 0;
            break;
    }
}

void i_prTextureSetDataOnGPU(prTextureData* texture) {
    GLenum format, internalFomrat;
    i_prTextureComputeFormats(texture, &format, &internalFomrat);

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrappingMode);	
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrappingMode);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (texture->pixelated ? GL_NEAREST : GL_LINEAR));
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (texture->pixelated ? GL_NEAREST : GL_LINEAR));

    texture->context->PixelStorei(GL_UNPACK_ALIGNMENT, 1);
    texture->context->TexImage2D(GL_TEXTURE_2D, 0, internalFomrat, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->textureData);
    texture->context->GenerateMipmap(GL_TEXTURE_2D);

    texture->context->BindTexture(GL_TEXTURE_2D, 0);
}

void i_prTextureCreateOnGPU(prTextureData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prTextureCreateOnGPU: Creating texture buffer object. Width: %i Height: %i Channels: %i", texture->width, texture->height, texture->channels);

    texture->context->GenTextures(1, &texture->TBO);
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
