#include <PR/cubeMapInternal.h>

#include <PR/defines.h>

#include <glad/gl.h>
#include <PR/cubeMap.h>
#include <PR/logger.h>

void i_prCubeMapComputeFormats(prCubeMapData* cubeMap, int side, GLenum* format, GLint* internalFormat) {
    switch(cubeMap->format[side]) {
        case PR_FORMAT_R:
            *format = GL_RED;
            *internalFormat = GL_RED;
            break;

        case PR_FORMAT_G:
            *format = GL_GREEN;
            *internalFormat = GL_GREEN;
            break;

        case PR_FORMAT_B:
            *format = GL_BLUE;
            *internalFormat = GL_BLUE;
            break;

        case PR_FORMAT_A:
            *format = GL_ALPHA;
            *internalFormat = GL_ALPHA;
            break;

        case PR_FORMAT_RGB:
            *format = GL_RGB;
            *internalFormat = GL_RGB16F;
            break;

        case PR_FORMAT_RGBA:
            *format = GL_RGBA;
            *internalFormat = GL_RGBA32F;
            break;

        case PR_FORMAT_STENCIL:
            *format = GL_STENCIL_INDEX;
            *internalFormat = GL_STENCIL_INDEX8;
            break;

        case PR_FORMAT_DEPTH:
            *format = GL_DEPTH_COMPONENT;
            *internalFormat = GL_DEPTH_COMPONENT24;
            break;

        case PR_FORMAT_DEPTH_STENCIL:
            *format = GL_DEPTH24_STENCIL8;
            *internalFormat = GL_UNSIGNED_INT_24_8;
            break;

        default:
            *format = GL_RGB;
            *internalFormat = GL_RGB16F;
            break;
    }
}

void i_prCubeMapSetDataAllOnGPU(prCubeMapData* cubeMap) {
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_WRAP_S, cubeMap->wrappingMode);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_WRAP_T, cubeMap->wrappingMode);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_WRAP_R, cubeMap->wrappingMode);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_MIN_FILTER, cubeMap->filter);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_MAG_FILTER, cubeMap->filter);

    cubeMap->context->PixelStorei(GL_UNPACK_ALIGNMENT, 1);

    cubeMap->context->BindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->TBO);
    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        GLenum format;
        GLint internalFomrat;
        i_prCubeMapComputeFormats(cubeMap, i, &format, &internalFomrat);
        cubeMap->context->TexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, internalFomrat, cubeMap->width[i], cubeMap->height[i], 0, format, GL_UNSIGNED_BYTE, cubeMap->textureData[i]
        );
    }
    cubeMap->context->BindTexture(GL_TEXTURE_CUBE_MAP, 0);

    cubeMap->context->GenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void i_prCubeMapSetDataOnGPU(prCubeMapData* cubeMap, int side) {
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_WRAP_S, cubeMap->wrappingMode);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_WRAP_T, cubeMap->wrappingMode);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_WRAP_R, cubeMap->wrappingMode);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_MIN_FILTER, cubeMap->filter);
    cubeMap->context->TextureParameteri(cubeMap->TBO, GL_TEXTURE_MAG_FILTER, cubeMap->filter);

    cubeMap->context->PixelStorei(GL_UNPACK_ALIGNMENT, 1);

    cubeMap->context->BindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->TBO);
    GLenum format;
    GLint internalFomrat;
    i_prCubeMapComputeFormats(cubeMap, side, &format, &internalFomrat);
    cubeMap->context->TexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
        0, internalFomrat, cubeMap->width[side], cubeMap->height[side], 0, format, GL_UNSIGNED_BYTE, cubeMap->textureData[side]
    );
    cubeMap->context->BindTexture(GL_TEXTURE_CUBE_MAP, 0);

    cubeMap->context->GenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void i_prCubeMapCreateOnGPU(prCubeMapData* cubeMap) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prCubeMapCreateOnGPU: Creating cube map buffer object");

    cubeMap->context->CreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cubeMap->TBO);
    if(!cubeMap->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prCubeMapCreateOnGPU: Failed to create cube map buffer object. Aborting operation, nothing was modified");
        return;
    }

    i_prCubeMapSetDataAllOnGPU(cubeMap);
}

void i_prCubeMapDestroyOnGPU(prCubeMapData* cubeMap) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prCubeMapDestroyOnGPU: Destroying cube map buffer object");

    cubeMap->context->DeleteTextures(1, &cubeMap->TBO);
}

void i_prCubeMapUpdateAllOnGPU(prCubeMapData* cubeMap) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prCubeMapUpdateAllOnGPU: Updating all faces of cube map buffer object. Width: %i Height: %i Channels: %i", cubeMap->width[0], cubeMap->height[0], cubeMap->channels[0]);

    if(!cubeMap->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prCubeMapUpdateAllOnGPU: Failed to update all faces of cube map buffer object, invalid ID. Aborting operation, nothing was modified");
        return;
    }

    i_prCubeMapSetDataAllOnGPU(cubeMap);
}

void i_prCubeMapUpdateOnGPU(prCubeMapData* cubeMap, int side) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prCubeMapUpdateOnGPU: Updating face %i of cube map buffer object. Width: %i Height: %i Channels: %i", side, cubeMap->width[side], cubeMap->height[side], cubeMap->channels[side]);

    if(!cubeMap->TBO) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prCubeMapUpdateOnGPU: Failed to update face %i of cube map buffer object, invalid ID. Aborting operation, nothing was modified", side);
        return;
    }

    i_prCubeMapSetDataOnGPU(cubeMap, side);
}
