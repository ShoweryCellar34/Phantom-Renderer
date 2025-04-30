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

void prTextureUpdate(prTextureData* texture, int format, int filter, int wrappingMode, GLubyte rawTextureData[], size_t rawTextureDataCount, GLint width, GLint height) {
    if(!rawTextureDataCount && rawTextureData) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Texture data count not zero while texture data is NULL. Assuming no texture data, texture data will be NULL");
    }
    if(rawTextureData && (width == 0 || height == 0)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Width and/or height provided in conjunction with texture data provided. Assuming raw, unconpressed texture data to be passed directly to GPU");
    }

    if((wrappingMode != PR_WRAPPING_REPEAT) && (wrappingMode != PR_WRAPPING_REPEAT_MIRRORED) && 
       (wrappingMode != PR_WRAPPING_EDGE) && (wrappingMode != PR_WRAPPING_COLOR)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Invalid wrapping mode for texture (was %i), using repeating wrapping mode", wrappingMode);
        texture->wrappingMode = PR_WRAPPING_REPEAT;
    } else {
        texture->wrappingMode = wrappingMode;
    }

    if(filter != PR_FILTER_LINEAR && filter != PR_FILTER_NEAREST) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureSetFilter: Invalid filter mode (was %i), using linear filtering", filter);
        texture->filter = PR_FILTER_LINEAR;
    } else {
        texture->filter = filter;
    }

    if((format != PR_FORMAT_A) && (format != PR_FORMAT_G) && (format != PR_FORMAT_B) &&
        (format != PR_FORMAT_RGB) && (format != PR_FORMAT_RGBA) &&
        (format != PR_FORMAT_STENCIL) && (format != PR_FORMAT_DEPTH) && (format != PR_FORMAT_DEPTH_STENCIL) &&
        (format != PR_FORMAT_AUTO)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prTextureUpdate: Invalid format for texture (was %i), using PR_FORMAT_RGB type", format);
        texture->format = PR_FORMAT_RGB;
    } else {
        texture->format = format;
    }

    unsigned char* temp = NULL;
    if(rawTextureData && (!width || !height)) {
        temp = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &texture->width, &texture->height, &texture->channels, 0);
        if(!temp) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prTextureUpdate: Texture data failed to unpack. Aborting operation, nothing was modified");
            return;
        }
    } else if(width || height) {
        temp = prMalloc(rawTextureDataCount);
        prMemcpy(temp, (void*)rawTextureData, rawTextureDataCount);
        texture->width = width;
        texture->height = height;
        texture->channels = 0;
    } else {
        texture->width = width;
        texture->height = height;
        texture->channels = 0;
    }

    if(format == PR_FORMAT_AUTO) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prTextureUpdate: Automatically determining texture format based on channel count (%d channels)", texture->channels);
        switch(texture->channels) {
            case 3:
                texture->format = PR_FORMAT_RGB;
                break;

            case 4:
                texture->format = PR_FORMAT_RGBA;
                break;

            default:
                texture->format = PR_FORMAT_RGB;
                break;
        }
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

void prTextureBindImage(prTextureData* texture, unsigned int index, unsigned int mipmapLevel, unsigned int access, unsigned int format) {
    texture->context->BindImageTexture(index, texture->TBO, mipmapLevel, GL_FALSE, 0, access, format);
}
