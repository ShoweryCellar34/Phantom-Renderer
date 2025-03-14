#pragma once

#include <stddef.h>
#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prTextureData prTextureData;
typedef struct prCamera prCamera;

typedef struct prMeshData {
    GladGLContext* context;
    prTextureData* texture;
    GLfloat* vertices;
    GLfloat* textureCoordinates;
    GLfloat* vertexColor;
    GLuint* indices;
    GLuint verticesCount, textureCoordinatesCount, vertexColorCount, indicesCount;
    GLuint VBO, VAO, EBO;
    GLfloat* GPUReadyBuffer;
    GLuint GPUReadyBufferCount;
    float mixRatio;
} prMeshData;

prMeshData* prMeshCreate();

void prMeshDestroy(prMeshData* mesh);

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context);

void prMeshLinkTexture(prMeshData* mesh, prTextureData* texture);

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount, GLfloat vertexColor[], size_t vertexColorCount);

void prMeshTextureToColorRatio(prMeshData* mesh, float mixRatio);

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera, unsigned int shaderProgram);
