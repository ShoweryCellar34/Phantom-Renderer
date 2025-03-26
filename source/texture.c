#include <PR/texture.h>

#include <PR/defines.h>

#include <PR/textureInternal.h>

#define STBI_MALLOC(size) prMalloc(size)
#define STBI_REALLOC(size) prRealloc(size)
#define STBI_FREE(size) prFree(size)

#include <stb_image.h>
#include <PR/memory.h>
#include <PR/error.h>

void prEnableImageFlip() {
    stbi_set_flip_vertically_on_load(1);
}

void prDisableImageFlip() {
    stbi_set_flip_vertically_on_load(0);
}

prTextureData* prTextureCreate() {
    prTextureData* texture = prMalloc(sizeof(prTextureData));

    texture->textureData = NULL;
    texture->wrappingMode = PR_TEXTURE_WRAPPING_EDGE;
    texture->pixelated = false;
    texture->width = 0;
    texture->height = 0;
    texture->channels = 0;
    texture->TBO = 0;

    return texture;
}

void prTextureDestroy(prTextureData* texture) {
    if(texture->TBO) {
        i_prTextureDestroyOnGPU(texture);
    }

    if(texture->textureData) {
        prFree(texture->textureData);
    }

    prFree(texture);
}

void prTextureLinkContext(prTextureData* texture, GladGLContext* context) {
    if(texture->context && texture->textureData) {
        i_prTextureDestroyOnGPU(texture);
    }
    texture->context = context;
    if(texture->context && texture->textureData) {
        i_prTextureCreateOnGPU(texture);
    }
}

void prTextureUpdate(prTextureData* texture, int wrappingMode, GLubyte rawTextureData[], size_t rawTextureDataCount) {
    if(!rawTextureDataCount) {
        prError(PR_INVALID_DATA_ERROR, "Texture data count cannot be zero. Aborting operation, nothing was modified");
    } else if(!rawTextureData) {
        prError(PR_INVALID_DATA_ERROR, "Texture data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    texture->textureData = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &texture->width, &texture->height, &texture->channels, 0);
    if(!texture->textureData) {
        prError(PR_INVALID_DATA_ERROR, "Texture data failed to unpack. Aborting operation, nothing was modified");
        return;
    }

    if((wrappingMode != PR_TEXTURE_WRAPPING_REPEAT) & (wrappingMode != PR_TEXTURE_WRAPPING_REPEAT_MIRRORED) & (wrappingMode != PR_TEXTURE_WRAPPING_EDGE) & (wrappingMode != PR_TEXTURE_WRAPPING_COLOR)) {
        prLogWarning("[DATA]", "Invalid wrapping mode for texture, using repeating wrapping mode");
        texture->wrappingMode = PR_TEXTURE_WRAPPING_REPEAT;
    } else {
        texture->wrappingMode = wrappingMode;
    }

    if(texture->textureData) {
        stbi_image_free(texture->textureData);
        texture->textureData = NULL;
    }

    if(texture->context && !texture->TBO) {
        i_prTextureCreateOnGPU(texture);
    } else if(texture->context) {
        i_prTextureUpdateOnGPU(texture);
    }
}

void prTextureSetPixelated(prTextureData* texture, bool pixelated) {
    texture->pixelated = pixelated;
}
