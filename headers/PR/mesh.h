#pragma once

#include <PR/defines.h>

#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prCamera prCamera;
typedef struct prShaderData prShaderData;

typedef struct prMeshData {
    GladGLContext* context;
    GLfloat* vertices;
    GLfloat* normals;
    GLfloat* textureCoordinates;
    GLuint* indices;
    GLsizei verticesCount, normalsCount, textureCoordinatesCount, indicesCount;
    GLuint VBO, VAO, EBO;
    GLfloat* GPUReadyBuffer;
    GLsizeiptr GPUReadyBufferCount;
} prMeshData;

prMeshData* prMeshCreate();

void prMeshDestroy(prMeshData* mesh);

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context);

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount,
    GLfloat normals[], size_t normalsCount,
    GLfloat textureCoordinates[], size_t textureCoordinatesCount,
    GLuint indices[], size_t indicesCount);

void prMeshDraw(prMeshData* mesh);
