#include <PR/texture.h>

#include <PR/defines.h>

#include <PR/textureInternal.h>

#define STBI_MALLOC(size) prMalloc(size)
#define STBI_REALLOC(size) prRealloc(size)
#define STBI_FREE(size) prFree(size)

#include <stb_image.h>
#include <PR/memory.h>
#include <PR/logger.h>

void prEnableImageFlip() {
    stbi_set_flip_vertically_on_load(1);
}

void prDisableImageFlip() {
    stbi_set_flip_vertically_on_load(0);
}

prTextureData* prTextureCreate() {
    prTextureData* texture = prCalloc(1, sizeof(prTextureData));

    texture->wrappingMode = PR_WRAPPING_EDGE;

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
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Texture data count cannot be zero. Aborting operation, nothing was modified");
    } else if(!rawTextureData) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Texture data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if((wrappingMode != PR_WRAPPING_REPEAT) & (wrappingMode != PR_WRAPPING_REPEAT_MIRRORED) & (wrappingMode != PR_WRAPPING_EDGE) & (wrappingMode != PR_WRAPPING_COLOR)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "Invalid wrapping mode for texture (was %i), using repeating wrapping mode", wrappingMode);
        texture->wrappingMode = PR_WRAPPING_REPEAT;
    } else {
        texture->wrappingMode = wrappingMode;
    }

    unsigned char* temp = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &texture->width, &texture->height, &texture->channels, 0);
    if(!temp) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "Texture data failed to unpack. Aborting operation, nothing was modified");
        return;
    }

    if(texture->textureData) {
        stbi_image_free(texture->textureData);
        texture->textureData = NULL;
    }

    texture->textureData = temp;

    if(texture->context && !texture->TBO) {
        i_prTextureCreateOnGPU(texture);
    } else if(texture->context) {
        i_prTextureUpdateOnGPU(texture);
    }
}

void prTextureSetPixelated(prTextureData* texture, bool pixelated) {
    texture->pixelated = pixelated;
}
