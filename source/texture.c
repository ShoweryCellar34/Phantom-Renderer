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

    return texture;
}

void prTextureDestroy(prTextureData* texture) {
    if(texture->TBO) {
        i_prTextureDestroyOnGPU(texture);
    }

    if(texture->textureData) {
        stbi_image_free(texture->textureData);
    }

    prFree(texture);
}

void prTextureLinkContext(prTextureData* texture, GladGLContext* context) {
    if(texture->context && texture->TBO) {
        i_prTextureDestroyOnGPU(texture);
    }
    texture->context = context;
    if(texture->context && texture->textureData) {
        i_prTextureCreateOnGPU(texture);
    }
}

void prTextureUpdate(prTextureData* texture, int type, int wrappingMode, GLubyte rawTextureData[], size_t rawTextureDataCount, GLint width, GLint height) {
    if(!rawTextureDataCount && rawTextureData) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Texture data count not zero while texture data is NULL. Assuming no texture data, texture data will be NULL");
    }
    if(((bool)rawTextureData & width) | ((bool)rawTextureData & height)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Width and height must be non-zero if texture data is provided. Assuming texture data, ignoring width and height");
    }

    if((wrappingMode != PR_WRAPPING_REPEAT) & (wrappingMode != PR_WRAPPING_REPEAT_MIRRORED) & (wrappingMode != PR_WRAPPING_EDGE) & (wrappingMode != PR_WRAPPING_COLOR)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Invalid wrapping mode for texture (was %i), using repeating wrapping mode", wrappingMode);
        texture->wrappingMode = PR_WRAPPING_REPEAT;
    } else {
        texture->wrappingMode = wrappingMode;
    }

    if((type != PR_COLOR) & (type != PR_DEPTH) & (type != PR_STENCIL) & (type != PR_DEPTH_STENCIL)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Invalid type for texture (was %i), using PR_COLOR type", type);
        texture->type = PR_COLOR;
    } else {
        texture->type = type;
    }

    unsigned char* temp = NULL;
    if(rawTextureData) {
        temp = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &texture->width, &texture->height, &texture->channels, 0);
        if(!temp) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureUpdate: Texture data failed to unpack. Aborting operation, nothing was modified");
            return;
        }
    } else {
        texture->width = width;
        texture->height = height;
        texture->channels = 0;
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
