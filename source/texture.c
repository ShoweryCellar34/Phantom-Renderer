#include <PR/texture.h>

#include <PR/textureInternal.h>

#include <stb_image.h>

prTextureData* prTextureCreate() {
    prTextureData* texture = malloc(sizeof(prTextureData));

    texture->textureData = NULL;
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;

    return texture;
}

void prTextureDestroy(prTextureData* texture) {
    i_prTextureDestroyOnGPUSide(texture);
    free(texture);
}

void prTextureLink(prTextureData* texture, prWindow* window) {
    if(texture->window && texture->textureData) {
        i_prTextureDestroyOnGPUSide(texture);
    }
    texture->window = window;
    if(texture->window && texture->textureData) {
        i_prTextureCreateOnGPUSide(texture);
    }
}

void prTextureUpdate(prTextureData* texture, unsigned char rawTextureData[], unsigned int rawTextureDataCount) {
    if(texture->textureData) {
        stbi_image_free(texture->textureData);
        texture->textureData = NULL;
    }

    texture->textureData = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &texture->width, &texture->height, &texture->channels, 0);

    if(texture->window && !texture->TBO) {
        i_prTextureCreateOnGPUSide(texture);
    } else if(texture->window) {
        i_prTextureUpdateOnGPUSide(texture);
    }
}
