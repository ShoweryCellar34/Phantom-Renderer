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

    GLint sizeAttribute[PR_MAX_VERTEX_ATTRIBUTES];
    GLenum typeAttribute[PR_MAX_VERTEX_ATTRIBUTES];
    GLboolean normalizedAttribute[PR_MAX_VERTEX_ATTRIBUTES];
    GLsizei strideAttribute[PR_MAX_VERTEX_ATTRIBUTES];
    void* offsetAttribute[PR_MAX_VERTEX_ATTRIBUTES];

    GLuint VBO, VAO, EBO;
} prMeshData;

prMeshData* prMeshCreate();

void prMeshDestroy(prMeshData* mesh);

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context);

void prMeshUpdate(prMeshData* mesh, void* GPUReadyBuffer, GLsizeiptr GPUReadyBufferSize, void* indices, GLsizeiptr indicesSize);

void prMeshSetVertexAttribute(prMeshData* mesh, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);

void prMeshDraw(prMeshData* mesh);

void prMeshDrawInstances(prMeshData* mesh, GLsizei count);
