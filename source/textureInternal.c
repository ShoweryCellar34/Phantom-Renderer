#include <PR/textureInternal.h>

#include <PR/error.h>

void i_prTextureCreateOnGPUSide(prTextureData* texture) {
    texture->context->GenTextures(1, &texture->TBO);
    if(!texture->TBO) {
        prError(PR_GL_ERROR, "Failed to create texture buffer object. Aborting operation, nothing was modified");
        return;
    }

    texture->context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, (texture->channels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, texture->textureData);
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
