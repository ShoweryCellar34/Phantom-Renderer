#include <PR/texture.h>

#include <PR/textureInternal.h>

#include <stb_image.h>
#include <PR/memory.h>
#include <PR/error.h>

void prEnableBlending(GladGLContext* context) {
    context->Enable(GL_BLEND);
    context->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void prEnableImageFlip() {
    stbi_set_flip_vertically_on_load(1);
}

void prDisableImageFlip() {
    stbi_set_flip_vertically_on_load(0);
}

void prDisableBlending(GladGLContext* context) {
    context->Disable(GL_BLEND);
}

prTextureData* prTextureCreate() {
    prTextureData* texture = prMalloc(sizeof(prTextureData));

    texture->textureData = NULL;
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;
    texture->TBO = 0;

    return texture;
}

void prTextureDestroy(prTextureData* texture) {
    i_prTextureDestroyOnGPUSide(texture);
    prFree(texture);
}

void prTextureLink(prTextureData* texture, GladGLContext* context) {
    if(texture->context && texture->textureData) {
        i_prTextureDestroyOnGPUSide(texture);
    }
    texture->context = context;
    if(texture->context && texture->textureData) {
        i_prTextureCreateOnGPUSide(texture);
    }
}

void prTextureUpdate(prTextureData* texture, unsigned char rawTextureData[], unsigned int rawTextureDataCount) {
    if(texture->textureData) {
        stbi_image_free(texture->textureData);
        texture->textureData = NULL;
    }

    if(!rawTextureDataCount) {
        prError(PR_INVALID_DATA_ERROR, "Texture data count ccannot be zero. Aborting operation, nothing was modified");
    } else if(!rawTextureData) {
        prError(PR_INVALID_DATA_ERROR, "Texture data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    texture->textureData = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &texture->width, &texture->height, &texture->channels, 0);
    if(!texture->textureData) {
        prError(PR_INVALID_DATA_ERROR, "Texture data failed to unpack. Aborting operation, nothing was modified");
        return;
    }

    if(texture->context && !texture->TBO) {
        i_prTextureCreateOnGPUSide(texture);
    } else if(texture->context) {
        i_prTextureUpdateOnGPUSide(texture);
    }
}
