#pragma once

#include <stddef.h>
#include <glad/gl.h>

typedef struct prTextureData prTextureData;

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

void prMeshLinkWindow(prMeshData* mesh, GladGLContext* context);

void prMeshLinkTexture(prMeshData* mesh, prTextureData* texture);

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount, GLfloat vertexColor[], size_t vertexColorCount);

void prMeshTextureToColorRatio(prMeshData* mesh, float mixRatio);

void prMeshDraw(prMeshData* mesh, unsigned int shaderProgram);
