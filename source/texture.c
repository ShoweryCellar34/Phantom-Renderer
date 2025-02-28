#include <PR/texture.h>

#include <PR/textureInternal.h>

#include <stb_image.h>

void prTextureInit(prTextureData* texture) {
    texture->textureData = NULL;
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;
}

void prTextureLink(prTextureData* texture, prWindow* window) {
    if(texture->window) {
        i_prTextureDestroyOnGPUSide(texture);
    }
    texture->window = window;
    if(texture->window && texture->textureData) {
        i_prTextureCreateOnGPUSide(texture);
    }
}

void prTextureUpdate(prTextureData* texture, unsigned char rawTextureData[], unsigned int rawTextureDataCount) {
    if(texture->textureData) {
        free(texture->textureData);
        texture->textureData = NULL;
    }

    // unsigned char* textureData = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &i_width, &i_height, &i_channels, 0);
    texture->textureData = stbi_load("awesomeface.png", &texture->width, &texture->height, &texture->channels, 0);

    if(texture->window) {
        i_prTextureUpdateOnGPUSide(texture);
    }

    if(texture->window && !texture->TBO) {
        i_prTextureCreateOnGPUSide(texture);
    } else if(texture->window) {
        i_prTextureUpdateOnGPUSide(texture);
    }
}
