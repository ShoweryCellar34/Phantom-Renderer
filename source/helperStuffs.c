#include <PR/helperStuffs.h>

#include <stdio.h>
#include <stdlib.h>
#include <PR/shader.h>
#include <PR/material.h>
#include <PR/texture.h>
#include <PR/textureInternal.h>
#include <PR/logger.h>
#include <PR/memory.h>

prShaderData* loadDefaultShader(GladGLContext* context) {
    prShaderData* shaderProgram = prShaderCreate();
    prShaderLinkContext(shaderProgram, context);
    prShaderUpdate(shaderProgram, BASE_VERTEX_SHADER, BASE_FRAGMENT_SHADER);

    return shaderProgram;
}

prMaterialData* makeMaterialOneTexture(prTextureData* texture) {
    prMaterialData* material = prMaterialCreate();

    prMaterialLinkAmbientMap(material, texture);
    prMaterialLinkDiffuseMap(material, texture);
    prMaterialLinkSpecularMap(material, texture);
    prMaterialLinkNormalMap(material, texture);

    return material;
}

prTextureData* loadTexture(GladGLContext* context, const char* path) {
    prTextureData* texture = prTextureCreate();

    FILE* textureFile = fopen(path, "rb");
    if(textureFile == NULL) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load file \"%s\"", path);
        exit(EXIT_FAILURE);
    }

    fseek(textureFile, 0L, SEEK_END);
    size_t textureFileSize = ftell(textureFile);
    fseek(textureFile, 0L, SEEK_SET);
    unsigned char* textureData = prMalloc(textureFileSize + 1);
    fread(textureData, textureFileSize, 1, textureFile);
    fclose(textureFile);

    prTextureLinkContext(texture, context);
    prTextureUpdate(texture, PR_FORMAT_RGBA, PR_WRAPPING_EDGE, textureData, textureFileSize, 0, 0);
    prFree(textureData);

    return texture;
}

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color) {
    prTextureData* texture = prTextureCreate();
    prTextureLinkContext(texture, context);

    texture->textureData = prMalloc(4 * sizeof(GLubyte));
    texture->textureData[0] = color[0] * 255.0f;
    texture->textureData[1] = color[1] * 255.0f;
    texture->textureData[2] = color[2] * 255.0f;
    texture->textureData[3] = color[3] * 255.0f;
    texture->channels = 4;
    texture->height = 1;
    texture->width = 1;

    i_prTextureCreateOnGPU(texture);

    return texture;
}

prTextureData* makeTextureCheckerboard(GladGLContext* context, size_t scale, float* color1, float* color2) {
    prTextureData* texture = prTextureCreate();
    prTextureLinkContext(texture, context);

    bool* template = prMalloc(scale * scale * sizeof(bool));
    for(size_t i = 0; i < scale * scale; i++) {
        static bool offset = false;
        if(!(i % scale) && !(scale % 2)) {
            offset = !offset;
        }

        if((i + offset) % 2) {
            template[i] = false;
        } else {
            template[i] = true;
        }
    }

    texture->textureData = prMalloc(scale * scale * 4 * sizeof(GLubyte));
    texture->wrappingMode = PR_WRAPPING_REPEAT;
    texture->pixelated = true;

    for(size_t i = 0; i < scale * scale; i++) {
        size_t index = i * 4;
        texture->textureData[index++] = (template[i] ? color1[0] * 255 : color2[0] * 255);
        texture->textureData[index++] = (template[i] ? color1[1] * 255 : color2[1] * 255);
        texture->textureData[index++] = (template[i] ? color1[2] * 255 : color2[2] * 255);
        texture->textureData[index] = (template[i] ? color1[3] * 255 : color2[3] * 255);
    }

    prFree(template);

    texture->format = PR_FORMAT_RGBA;
    texture->channels = 4;
    texture->height = scale;
    texture->width = scale;

    i_prTextureCreateOnGPU(texture);

    return texture;
}
