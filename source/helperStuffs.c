#include <PR/helperStuffs.h>

#include <stdio.h>
#include <stdlib.h>
#include <PR/texture.h>
#include <PR/cubeMap.h>
#include <PR/logger.h>
#include <PR/memory.h>
#include <PR/shader.h>
#include <PR/computeShader.h>

prTextureData* loadTexture(GladGLContext* context, unsigned int filtering, const char* path) {
    FILE* textureFile = fopen(path, "rb");
    if(textureFile == NULL) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load file \"%s\"", path);
        exit(EXIT_FAILURE);
    }

    fseek(textureFile, 0, SEEK_END);
    size_t textureFileSize = ftell(textureFile);
    rewind(textureFile);
    unsigned char* textureData = prMalloc(textureFileSize + 1);
    fread(textureData, 1, textureFileSize, textureFile);
    fclose(textureFile);

    prTextureData* texture = prTextureCreate();

    prTextureLinkContext(texture, context);
    prTextureUpdate(texture, PR_FORMAT_AUTO, PR_WRAPPING_EDGE, filtering, textureData, textureFileSize, 0, 0);
    prFree(textureData);

    return texture;
}

prTextureData* makeTextureSingleColor(GladGLContext* context, float color[4]) {
    prTextureData* texture = prTextureCreate();
    prTextureLinkContext(texture, context);

    GLubyte textureData[4] = {color[0] * 255.0f, color[1] * 255.0f, color[2] * 255.0f, color[3] * 255.0f};

    prTextureUpdate(texture, PR_FORMAT_RGBA, PR_WRAPPING_REPEAT, PR_FILTER_LINEAR, textureData, 4, 1, 1);

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

prCubeMapData* loadCubeMap(GladGLContext* context, unsigned int filtering, const char* paths[PR_CUBE_MAP_SIDES]) {
    size_t textureFileSize[PR_CUBE_MAP_SIDES] = {0, 0, 0, 0, 0, 0};
    unsigned char* textureData[PR_CUBE_MAP_SIDES] = {NULL, NULL, NULL, NULL, NULL, NULL};

    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        FILE* textureFile = fopen(paths[i], "rb");
        if(textureFile == NULL) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load file \"%s\"", paths[i]);
            exit(EXIT_FAILURE);
        }

        fseek(textureFile, 0, SEEK_END);
        textureFileSize[i] = ftell(textureFile);
        rewind(textureFile);
        textureData[i] = prMalloc(textureFileSize[i] + 1);
        fread(textureData[i], 1, textureFileSize[i], textureFile);
        fclose(textureFile);
    }

    prCubeMapData* cubeMap = prCubeMapCreate();
    prCubeMapLinkContext(cubeMap, context);
    prCubeMapUpdateAll(cubeMap,
        (GLenum[PR_CUBE_MAP_SIDES]){PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO, PR_FORMAT_AUTO},
        PR_WRAPPING_EDGE, filtering,
        (GLubyte*[PR_CUBE_MAP_SIDES]){textureData[0], textureData[1], textureData[2], textureData[3], textureData[4], textureData[5]},
        (size_t[PR_CUBE_MAP_SIDES]){textureFileSize[0], textureFileSize[1], textureFileSize[2], textureFileSize[3], textureFileSize[4], textureFileSize[5]},
        (GLsizei[PR_CUBE_MAP_SIDES]){0, 0, 0, 0, 0 ,0}, (GLsizei[PR_CUBE_MAP_SIDES]){0, 0, 0, 0, 0 ,0});

    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        prFree(textureData[i]);
    }

    return cubeMap;
}

prCubeMapData* makeCubeMapSingleColors(GladGLContext* context, float color[PR_CUBE_MAP_SIDES][4]) {
    unsigned char textureData[PR_CUBE_MAP_SIDES][4] = {
        {color[0][0] * 255.0f, color[0][1] * 255.0f, color[0][2] * 255.0f, color[0][3] * 255.0f},
        {color[1][0] * 255.0f, color[1][1] * 255.0f, color[1][2] * 255.0f, color[1][3] * 255.0f},
        {color[2][0] * 255.0f, color[2][1] * 255.0f, color[2][2] * 255.0f, color[2][3] * 255.0f},
        {color[3][0] * 255.0f, color[3][1] * 255.0f, color[3][2] * 255.0f, color[3][3] * 255.0f},
        {color[4][0] * 255.0f, color[4][1] * 255.0f, color[4][2] * 255.0f, color[4][3] * 255.0f},
        {color[5][0] * 255.0f, color[5][1] * 255.0f, color[5][2] * 255.0f, color[5][3] * 255.0f}
    };
    size_t textureFileSize[PR_CUBE_MAP_SIDES] = {4, 4, 4, 4, 4, 4};

    prCubeMapData* cubeMap = prCubeMapCreate();
    prCubeMapLinkContext(cubeMap, context);
    prCubeMapUpdateAll(cubeMap,
        (GLenum[PR_CUBE_MAP_SIDES]){PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA},
        PR_WRAPPING_EDGE, PR_FILTER_LINEAR,
        (GLubyte*[PR_CUBE_MAP_SIDES]){textureData[0], textureData[1], textureData[2], textureData[3], textureData[4], textureData[5]},
        (size_t[PR_CUBE_MAP_SIDES]){textureFileSize[0], textureFileSize[1], textureFileSize[2], textureFileSize[3], textureFileSize[4], textureFileSize[5]},
        (GLsizei[PR_CUBE_MAP_SIDES]){1, 1, 1, 1, 1, 1}, (GLsizei[PR_CUBE_MAP_SIDES]){1, 1, 1, 1, 1, 1});

    return cubeMap;
}

prCubeMapData* makeCubeMapCheckerboards(GladGLContext* context, size_t scale, float color1[PR_CUBE_MAP_SIDES][4], float color2[PR_CUBE_MAP_SIDES][4]) {
    prCubeMapData* cubeMap = prCubeMapCreate();
    prCubeMapLinkContext(cubeMap, context);

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

    GLubyte* textureData[PR_CUBE_MAP_SIDES] = {NULL, NULL, NULL, NULL, NULL, NULL};
    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        textureData[i] = prMalloc(scale * scale * 4 * sizeof(GLubyte));
    }
    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        for(size_t j = 0; j < scale * scale; j++) {
            size_t index = j * 4;
            textureData[i][index++] = (template[j] ? color1[i][0] * 255 : color2[i][0] * 255);
            textureData[i][index++] = (template[j] ? color1[i][1] * 255 : color2[i][1] * 255);
            textureData[i][index++] = (template[j] ? color1[i][2] * 255 : color2[i][2] * 255);
            textureData[i][index] = (template[j] ? color1[i][3] * 255 : color2[i][3] * 255);
        }
    }

    prFree(template);

    prCubeMapUpdateAll(cubeMap,
        (GLenum[PR_CUBE_MAP_SIDES]){PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA, PR_FORMAT_RGBA},
        PR_WRAPPING_EDGE, PR_FILTER_NEAREST,
        (GLubyte*[PR_CUBE_MAP_SIDES]){textureData[0], textureData[1], textureData[2], textureData[3], textureData[4], textureData[5]},
        (size_t[PR_CUBE_MAP_SIDES]){scale * scale * 4, scale * scale * 4, scale * scale * 4, scale * scale * 4, scale * scale * 4, scale * scale * 4},
        (GLsizei[PR_CUBE_MAP_SIDES]){scale, scale, scale, scale, scale, scale},
        (GLsizei[PR_CUBE_MAP_SIDES]){scale, scale, scale, scale, scale, scale});

    
    for(int i = 0; i < PR_CUBE_MAP_SIDES; i++) {
        prFree(textureData[i]);
    }

    return cubeMap;
}

prShaderData* loadShader(GladGLContext* context, const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
    prShaderData* shader = prShaderCreate();
    prShaderLinkContext(shader, context);

    char* vertexData = NULL;
    char* geometryData = NULL;
    char* fragmentData = NULL;

    FILE* file = fopen(vertexPath, "rb");
    if(file == NULL) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load vertex shader file \"%s\"", vertexPath);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    vertexData = prMalloc(fileSize + 1);
    fread(vertexData, 1, fileSize, file);
    vertexData[fileSize] = '\0';
    fclose(file);

    file = fopen(fragmentPath, "rb");
    if(file == NULL) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load fragment shader file \"%s\"", fragmentPath);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);
    fragmentData = prMalloc(fileSize + 1);
    fread(fragmentData, 1, fileSize, file);
    fragmentData[fileSize] = '\0';
    fclose(file);

    if(geometryPath) {
        file = fopen(geometryPath, "rb");
        if(file == NULL) {
            prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load geometry shader file \"%s\"", geometryPath);
            exit(EXIT_FAILURE);
        }
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        rewind(file);
        geometryData = prMalloc(fileSize + 1);
        fread(geometryData, 1, fileSize, file);
        geometryData[fileSize] = '\0';
        fclose(file);
    }

    prShaderUpdate(shader, vertexData, fragmentData, geometryData);

    prFree(vertexData);
    prFree(fragmentData);
    if(geometryData) {
        prFree(geometryData);
    }

    return shader;
}

prComputeShaderData* loadComputeShader(GladGLContext* context, const char* path) {
    prComputeShaderData* computeShader = prComputeShaderCreate();
    prComputeShaderLinkContext(computeShader, context);

    FILE* file = fopen(path, "rb");
    if(file == NULL) {
        prLogEvent(PR_EVENT_USER, PR_LOG_ERROR, "Failed to load compute shader file \"%s\"", path);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
    char* computeData = prMalloc(fileSize + 1);
    fread(computeData, 1, fileSize, file);
    computeData[fileSize] = '\0';
    fclose(file);

    prComputeShaderUpdate(computeShader, computeData);

    prFree(computeData);

    return computeShader;
}
