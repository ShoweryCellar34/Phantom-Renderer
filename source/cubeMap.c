#include <PR/cubeMap.h>

#include <PR/cubeMapInternal.h>

#include <stb_image.h>
#include <PR/logger.h>
#include <PR/memory.h>

prCubeMapData* prCubeMapCreate() {
    prCubeMapData* cubeMap = prCalloc(1, sizeof(prCubeMapData));

    return cubeMap;
}

void prCubeMapDestroy(prCubeMapData* cubeMap) {
    if(cubeMap->TBO) {
        i_prCubeMapDestroyOnGPU(cubeMap);
    }

    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        if(cubeMap->textureData[i]) {
            stbi_image_free(cubeMap->textureData[i]);
        }
    }

    prFree(cubeMap);
}

void prCubeMapLinkContext(prCubeMapData* cubeMap, GladGLContext* context) {
    if(cubeMap->context && cubeMap->TBO) {
        i_prCubeMapDestroyOnGPU(cubeMap);
    }
    cubeMap->context = context;
    if(cubeMap->context) {
        i_prCubeMapCreateOnGPU(cubeMap);
    }
}

void prCubeMapUpdateAll(prCubeMapData* cubeMap, GLenum format[PR_CUBE_MAP_SIDES], GLint wrappingMode, GLint filter, GLubyte* rawTextureData[PR_CUBE_MAP_SIDES], size_t rawTextureDataCount[PR_CUBE_MAP_SIDES], GLsizei width[PR_CUBE_MAP_SIDES], GLsizei height[PR_CUBE_MAP_SIDES]) {
    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        if(!rawTextureDataCount && rawTextureData) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdateAll: [Face %i] Cube map face data count not zero while cube map face data is NULL. Assuming no texture data, texture data will be NULL");
        }
        if(rawTextureData && (width[i] || height[i])) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_INFO, "prCubeMapUpdateAll: [Face %i] Width and/or height provided in conjunction with cube map face data was provided. Assuming raw, unconpressed texture data to be passed directly to GPU");
        }

        if(!rawTextureData[i]) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdateAll: [Face %i] Cube map face data NULL, aborting operation, nothing was modified", i);
        }

        if((format[i] != PR_FORMAT_A) && (format[i] != PR_FORMAT_G) && (format[i] != PR_FORMAT_B) &&
            (format[i] != PR_FORMAT_RGB) && (format[i] != PR_FORMAT_RGBA) &&
            (format[i] != PR_FORMAT_STENCIL) && (format[i] != PR_FORMAT_DEPTH) && (format[i] != PR_FORMAT_DEPTH_STENCIL) &&
            (format[i] != PR_FORMAT_AUTO)
        ) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdateAll: [Face %i] Invalid format for cube map face (was %i), using PR_FORMAT_RGB type", i, format);
            cubeMap->format[i] = PR_FORMAT_RGB;
        } else {
            cubeMap->format[i] = format[i];
        }

        unsigned char* temp = NULL;
        if(rawTextureData && (!width[i] || !height[i])) {
            temp = stbi_load_from_memory(rawTextureData[i], rawTextureDataCount[i], &cubeMap->width[i], &cubeMap->height[i], &cubeMap->channels[i], 0);
            if(!temp) {
                prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prCubeMapUpdateAll: [Face %i] Cube map face data failed to unpack. Aborting operation, nothing was modified", i);
                return;
            }
        } else if(!rawTextureData && (width[i] || height[i])) {
            temp = NULL;
            cubeMap->width[i] = width[i];
            cubeMap->height[i] = height[i];
            cubeMap->channels[i] = 0;
        } else if(rawTextureData && (width || height)) {
            temp = prMalloc(rawTextureDataCount[i]);
            prMemcpy(temp, (void*)rawTextureData, rawTextureDataCount[i]);
            cubeMap->width[i] = width[i];
            cubeMap->height[i] = height[i];
            cubeMap->channels[i] = 0;
        }

        if(format[i] == PR_FORMAT_AUTO) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prCubeMapUpdateAll: [Face %i] Automatically determining cube map face format based on channel count (%d channels)", i, cubeMap->channels[i]);
            switch(cubeMap->channels[i]) {
                case 3:
                    cubeMap->format[i] = PR_FORMAT_RGB;
                    break;

                case 4:
                    cubeMap->format[i] = PR_FORMAT_RGBA;
                    break;

                default:
                    cubeMap->format[i] = PR_FORMAT_RGB;
                    break;
            }
        }

        if(cubeMap->textureData[i]) {
            stbi_image_free(cubeMap->textureData[i]);
            cubeMap->textureData[i] = NULL;
        }

        cubeMap->textureData[i] = temp;
    }

    if((wrappingMode != PR_WRAPPING_REPEAT) && (wrappingMode != PR_WRAPPING_REPEAT_MIRRORED) && 
    (wrappingMode != PR_WRAPPING_EDGE) && (wrappingMode != PR_WRAPPING_COLOR)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdateAll: Invalid wrapping mode for cube map (was %i), using PR_WRAPPING_EDGE", wrappingMode);
        cubeMap->wrappingMode = PR_WRAPPING_EDGE;
    } else {
        cubeMap->wrappingMode = wrappingMode;
    }

    if(filter != PR_FILTER_LINEAR && filter != PR_FILTER_NEAREST) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdateAll: Invalid filter mode for cube map (was %i), using PR_FILTER_LINEAR", filter);
        cubeMap->filter = PR_FILTER_LINEAR;
    } else {
        cubeMap->filter = filter;
    }

    if(cubeMap->context && !cubeMap->TBO) {
        i_prCubeMapCreateOnGPU(cubeMap);
    } else if(cubeMap->context) {
        i_prCubeMapUpdateAllOnGPU(cubeMap);
    }
}

void prCubeMapUpdate(prCubeMapData* cubeMap, int side, GLenum format, GLint wrappingMode, GLint filter, GLubyte* rawTextureData, size_t rawTextureDataCount, GLsizei width, GLsizei height) {
    if(!rawTextureDataCount && rawTextureData) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdate: [Face %i] Cube map face data count not zero while cube map face data is NULL. Assuming no texture data, texture data will be NULL", side);
    }
    if(rawTextureData && (width == 0 || height == 0)) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_INFO, "prCubeMapUpdate: [Face %i] Width and/or height provided in conjunction with cube map face data was provided. Assuming raw, unconpressed texture data to be passed directly to GPU", side);
    }

    if((format != PR_FORMAT_A) && (format != PR_FORMAT_G) && (format != PR_FORMAT_B) &&
        (format != PR_FORMAT_RGB) && (format != PR_FORMAT_RGBA) &&
        (format != PR_FORMAT_STENCIL) && (format != PR_FORMAT_DEPTH) && (format != PR_FORMAT_DEPTH_STENCIL) &&
        (format != PR_FORMAT_AUTO)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdate: [Face %i] Invalid format for cube map face (was %i), using PR_FORMAT_RGB type", side, format);
        cubeMap->format[side] = PR_FORMAT_RGB;
    } else {
        cubeMap->format[side] = format;
    }

    if((wrappingMode != PR_WRAPPING_REPEAT) && (wrappingMode != PR_WRAPPING_REPEAT_MIRRORED) && 
    (wrappingMode != PR_WRAPPING_EDGE) && (wrappingMode != PR_WRAPPING_COLOR)
    ) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdate: Invalid wrapping mode for cube map (was %i), using PR_WRAPPING_EDGE", wrappingMode);
        cubeMap->wrappingMode = PR_WRAPPING_EDGE;
    } else {
        cubeMap->wrappingMode = wrappingMode;
    }

    if(filter != PR_FILTER_LINEAR && filter != PR_FILTER_NEAREST) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_WARNING, "prCubeMapUpdate: Invalid filter mode for cube map (was %i), using PR_FILTER_LINEAR", filter);
        cubeMap->filter = PR_FILTER_LINEAR;
    } else {
        cubeMap->filter = filter;
    }

    unsigned char* temp = NULL;
    if(rawTextureData && (!width || !height)) {
        temp = stbi_load_from_memory(rawTextureData, rawTextureDataCount, &cubeMap->width[side], &cubeMap->height[side], &cubeMap->channels[side], 0);
        if(!temp) {
            prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prCubeMapUpdate: [Face %i] Cube map face data failed to unpack. Aborting operation, nothing was modified", side);
            return;
        }
    } else if(!rawTextureData && (width || height)) {
        temp = NULL;
        cubeMap->width[side] = width;
        cubeMap->height[side] = height;
        cubeMap->channels[side] = 0;
    } else if(rawTextureData && (width || height)) {
        temp = prMalloc(rawTextureDataCount);
        prMemcpy(temp, (void*)rawTextureData, rawTextureDataCount);
        cubeMap->width[side] = width;
        cubeMap->height[side] = height;
        cubeMap->channels[side] = 0;
    }

    if(format == PR_FORMAT_AUTO) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_TRACE, "prCubeMapUpdate: [Face %i] Automatically determining cube map face format based on channel count (%d channels)", side, cubeMap->channels[side]);
        switch(cubeMap->channels[side]) {
            case 3:
                cubeMap->format[side] = PR_FORMAT_RGB;
                break;

            case 4:
                cubeMap->format[side] = PR_FORMAT_RGBA;
                break;

            default:
                cubeMap->format[side] = PR_FORMAT_RGB;
                break;
        }
    }

    if(cubeMap->textureData[side]) {
        stbi_image_free(cubeMap->textureData[side]);
        cubeMap->textureData[side] = NULL;
    }

    cubeMap->textureData[side] = temp;

    if(cubeMap->context && !cubeMap->TBO) {
        i_prCubeMapCreateOnGPU(cubeMap);
    } else if(cubeMap->context) {
        i_prCubeMapUpdateOnGPU(cubeMap, side);
    }
}

void prCubeMapBindTexture(prCubeMapData* cubeMap, unsigned int unit) {
    if(!cubeMap->TBO) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prCubeMapBindTexture: Attempt to bind cube map that has not been created on GPU. Aborting operation");
        return;
    }

    if(!cubeMap->context) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prCubeMapBindTexture: Attempt to bind cube map without OpenGL context. Aborting operation");
        return;
    }

    cubeMap->context->ActiveTexture(GL_TEXTURE0 + unit);
    cubeMap->context->BindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->TBO);
}
