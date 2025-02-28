#pragma once

#include <stddef.h>
#include <glad/gl.h>

struct i_prMeshData {
    GLfloat* vertices;
    GLfloat* textureCoordinates;
    GLuint* indices;
    GLuint verticesCount, textureCoordinatesCount, indicesCount;
    GLuint VBO, VAO, EBO;
    GLfloat* GPUArray;
    GLuint GPUArrayCount;
};

typedef struct i_prMeshData prMeshData;

void prInitMesh(prMeshData* mesh);

void prUpdateMesh(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount);
