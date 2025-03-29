#include <PR/textureInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/logger.h>
#include <PR/texture.h>

void i_prTextureCreateOnGPU(prTextureData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "Creating texture buffer object. Width: %i Height: %i Channels: %i", texture->width, texture->height, texture->channels);

    texture->context->GenTextures(1, &texture->TBO);
    if(!texture->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "Failed to create texture buffer object. Aborting operation, nothing was modified");
        return;
    }

    int format = GL_RGBA;
    switch(texture->channels) {
        case 1:
            format = GL_ALPHA;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;
    }

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrappingMode);	
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrappingMode);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (texture->pixelated ? GL_NEAREST : GL_LINEAR));
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (texture->pixelated ? GL_NEAREST : GL_LINEAR));

    texture->context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->textureData);
    texture->context->GenerateMipmap(GL_TEXTURE_2D);

    texture->context->BindTexture(GL_TEXTURE_2D, 0);

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);
}

void i_prTextureUpdateOnGPU(prTextureData* texture) {
    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->wrappingMode);	
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->wrappingMode);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (texture->pixelated ? GL_NEAREST : GL_LINEAR));
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (texture->pixelated ? GL_NEAREST : GL_LINEAR));

    texture->context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->textureData);
    texture->context->GenerateMipmap(GL_TEXTURE_2D);
}

void i_prTextureDestroyOnGPU(prTextureData* texture) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "Destroying texture buffer object");

    texture->context->DeleteTextures(1, &texture->TBO);
}
