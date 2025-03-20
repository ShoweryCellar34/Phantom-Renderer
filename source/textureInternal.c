#include <PR/textureInternal.h>

#include <PR/defines.h>

#include <PR/error.h>

void i_prTextureCreateOnGPUSide(prTextureData* texture) {
    texture->context->GenTextures(1, &texture->TBO);
    if(!texture->TBO) {
        prError(PR_GL_ERROR, "Failed to create texture buffer object. Aborting operation, nothing was modified");
        return;
    }

    int format = GL_RGBA;
    switch(texture->channels) {
        case 1:
            format = GL_RED;
            break;

        case 2:
            format = GL_RG;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;
    }

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, texture->textureData);
    texture->context->GenerateMipmap(GL_TEXTURE_2D);

    texture->context->BindTexture(GL_TEXTURE_2D, 0);

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    prLogInfo("[GL]", "Successfully created texture buffer object and set data");
}

void i_prTextureUpdateOnGPUSide(prTextureData* texture) {
    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    texture->context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->textureData);
    texture->context->GenerateMipmap(GL_TEXTURE_2D);
}

void i_prTextureDestroyOnGPUSide(prTextureData* texture) {
    texture->context->DeleteTextures(1, &texture->TBO);
}
