#include <PR/helperStuffs.h>

#include <stdio.h>
#include <stdlib.h>
#include <PR/shader.h>
#include <PR/texture.h>
#include <PR/cubeMap.h>
#include <PR/logger.h>
#include <PR/memory.h>

prTextureData* loadTexture(GladGLContext* context, unsigned int filtering, const char* path) {
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

    prTextureData* texture = prTextureCreate();

    prTextureLinkContext(texture, context);
    prTextureUpdate(texture, PR_FORMAT_AUTO, PR_WRAPPING_EDGE, filtering, textureData, textureFileSize, 0, 0);
    prFree(textureData);

    return texture;
}

prCubeMapData* loadCubeMap(GladGLContext* context, unsigned int filtering, const char* paths[PR_CUBE_MAP_SIDES]) {
    size_t textureFileSize[PR_CUBE_MAP_SIDES] = {0, 0, 0, 0, 0, 0};
    unsigned char* textureData[PR_CUBE_MAP_SIDES] = {NULL, NULL, NULL, NULL, NULL, NULL};

    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        FILE* textureFile = fopen(paths[i], "rb");
        if(textureFile == NULL) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load file \"%s\"", paths[i]);
            exit(EXIT_FAILURE);
        }

        fseek(textureFile, 0L, SEEK_END);
        textureFileSize[i] = ftell(textureFile);
        fseek(textureFile, 0L, SEEK_SET);
        textureData[i] = prMalloc(textureFileSize[i] + 1);
        fread(textureData, textureFileSize[i], 1, textureFile);
        fclose(textureFile);
    }

    prCubeMapData* cubeMap = prCubeMapCreate();
    prCubeMapLinkContext(cubeMap, context);
    prCubeMapUpdateAll(cubeMap,
        (GLenum[PR_CUBE_MAP_SIDES]){PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO},
        PR_WRAPPING_EDGE, PR_FILTER_LINEAR,
        (GLubyte*[PR_CUBE_MAP_SIDES]){textureData[0], textureData[1], textureData[2], textureData[3], textureData[4], textureData[5]},
        (size_t[PR_CUBE_MAP_SIDES]){textureFileSize[0], textureFileSize[1], textureFileSize[2], textureFileSize[3], textureFileSize[4], textureFileSize[5]});

    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        prFree(textureData[i]);
    }

    return cubeMap;
}

prTextureData* makeTextureSingleColor(GladGLContext* context, float* color) {
    prTextureData* texture = prTextureCreate();
    prTextureLinkContext(texture, context);

    GLubyte* textureData = prMalloc(4 * sizeof(GLubyte));
    textureData[0] = color[0] * 255.0f;
    textureData[1] = color[1] * 255.0f;
    textureData[2] = color[2] * 255.0f;
    textureData[3] = color[3] * 255.0f;

    prTextureUpdate(texture, PR_FORMAT_RGBA, PR_WRAPPING_REPEAT, PR_FILTER_LINEAR, textureData, 4, 1, 1);

    prFree(textureData);

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

    GLubyte* textureData = prMalloc(scale * scale * 4 * sizeof(GLubyte));
    for(size_t i = 0; i < scale * scale; i++) {
        size_t index = i * 4;
        textureData[index++] = (template[i] ? color1[0] * 255 : color2[0] * 255);
        textureData[index++] = (template[i] ? color1[1] * 255 : color2[1] * 255);
        textureData[index++] = (template[i] ? color1[2] * 255 : color2[2] * 255);
        textureData[index] = (template[i] ? color1[3] * 255 : color2[3] * 255);
    }

    prFree(template);

    prTextureUpdate(texture, PR_FORMAT_RGBA, PR_WRAPPING_REPEAT, PR_FILTER_NEAREST, textureData, scale * scale * 4, scale, scale);
    
    prFree(textureData);

    return texture;
}
