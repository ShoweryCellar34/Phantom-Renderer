#pragma once

#include <stddef.h>
#include <glad/gl.h>

typedef struct prWindow prWindow;

typedef struct prMeshData {
    prWindow* window;
    GLfloat* vertices;
    GLfloat* textureCoordinates;
    GLuint* indices;
    GLuint verticesCount, textureCoordinatesCount, indicesCount;
    GLuint VBO, VAO, EBO;
    GLfloat* GPUReadyBuffer;
    GLuint GPUReadyBufferCount;
} prMeshData;

prMeshData* prMeshCreate();

void prMeshDestroy(prMeshData* mesh);

void prMeshLink(prMeshData* mesh, prWindow* window);

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount);
