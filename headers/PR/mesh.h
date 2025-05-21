#pragma once

#include <PR/defines.h>

#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prCamera prCamera;
typedef struct prShaderData prShaderData;

typedef struct prMeshData {
    GladGLContext* context;
    void* GPUReadyBuffer;
    GLsizeiptr GPUReadyBufferSize;
    void* indices;
    GLsizei indicesSize;
    GLuint VBO, VAO, EBO;
} prMeshData;

prMeshData* prMeshCreate();

void prMeshDestroy(prMeshData* mesh);

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context);

void prMeshUpdate(prMeshData* mesh, void* GPUReadyBuffer, GLsizeiptr GPUReadyBufferSize, void* indices, GLsizeiptr indicesSize);

void prMeshSetVertexAttribute(prMeshData* mesh, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

void prMeshDraw(prMeshData* mesh);
