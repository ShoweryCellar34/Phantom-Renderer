#include <PR/textureInternal.h>

#include <PR/window.h>

void i_prTextureCreateOnGPUSide(prTextureData* texture) {
    GladGLContext* context = &texture->window->openglContext;

    context->GenTextures(1, &texture->TBO);
    context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    context->TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->textureData);
    context->GenerateMipmap(GL_TEXTURE_2D);

    context->BindTexture(GL_TEXTURE_2D, 0);

    context->BindTexture(GL_TEXTURE_2D, texture->TBO);
}

void i_prTextureUpdateOnGPUSide(prTextureData* texture) {
    GladGLContext* context = &texture->window->openglContext;

    context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    context->TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->textureData);
    context->GenerateMipmap(GL_TEXTURE_2D);
}

void i_prTextureDestroyOnGPUSide(prTextureData* texture) {
    GladGLContext* context = &texture->window->openglContext;

    context->DeleteTextures(1, &texture->TBO);
}
