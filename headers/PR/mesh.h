#ifndef PR_MESH_H
#define PR_MESH_H

#include <PR/defines.h>

#include <glad/gl.h>

typedef struct prCamera prCamera;
typedef struct prShaderData prShaderData;

typedef struct prMeshData {
    GladGLContext* context;
    PRMeshDrawFunction drawFunction;
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

void prMeshSetDrawFunction(prMeshData* mesh, PRMeshDrawFunction drawFunction);

void prMeshUpdate(prMeshData* mesh, void* GPUReadyBuffer, GLsizeiptr GPUReadyBufferSize, void* indices, GLsizeiptr indicesSize);

void prMeshSetVertexAttribute(prMeshData* mesh, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);

void prMeshDrawIndices(prMeshData* mesh, void* data);

void prMeshDrawIndicesInstances(prMeshData* mesh, GLsizei count, void* data);

void prMeshDraw(prMeshData* mesh, GLsizei verticesCount, void* data);

void prMeshDrawInstances(prMeshData* mesh, GLsizei verticesCount, GLsizei count, void* data);

#endif // PR_MESH_H
