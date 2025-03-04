#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <PR/memory.h>

prMeshData* prMeshCreate() {
    prMeshData* mesh = prMalloc(sizeof(prMeshData));

    mesh->context = NULL;
    mesh->vertices = NULL;
    mesh->textureCoordinates = NULL;
    mesh->indices = NULL;
    mesh->verticesCount = 0;
    mesh->textureCoordinatesCount = 0;
    mesh->indicesCount = 0;
    mesh->VBO = 0;
    mesh->VAO = 0;
    mesh->EBO = 0;
    mesh->GPUReadyBuffer = NULL;
    mesh->GPUReadyBufferCount = 0;

    return mesh;
}

void prMeshDestroy(prMeshData* mesh) {
    i_prMeshDestroyOnGPUSide(mesh);
    prFree(mesh);
}

void prMeshLink(prMeshData* mesh, GladGLContext* context) {
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshDestroyOnGPUSide(mesh);
    }
    mesh->context = context;
    if(mesh->context && mesh->GPUReadyBuffer) {
        i_prMeshCreateOnGPUSide(mesh);
    }
}

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount) {
    if(mesh->vertices) {
        prFree(mesh->vertices);
        prFree(mesh->indices);
        mesh->vertices = NULL;
        mesh->indices = NULL;
    }
    if(mesh->textureCoordinates) {
        prFree(mesh->textureCoordinates);
        mesh->textureCoordinates = NULL;
    }

    mesh->vertices = prMalloc(sizeof(GLfloat) * verticesCount);
    prMemcpy(mesh->vertices, vertices, sizeof(GLfloat) * verticesCount);
    mesh->verticesCount = verticesCount;

    mesh->indices = prMalloc(sizeof(GLuint) * indicesCount);
    prMemcpy(mesh->indices, indices, sizeof(GLuint) * indicesCount);
    mesh->indicesCount = indicesCount;

    if(textureCoordinatesCount && textureCoordinates) {
        mesh->textureCoordinates = prMalloc(sizeof(GLfloat) * textureCoordinatesCount);
        prMemcpy(mesh->textureCoordinates, textureCoordinates, sizeof(GLfloat) * textureCoordinatesCount);
        mesh->textureCoordinatesCount = textureCoordinatesCount;
    }
    mesh->textureCoordinatesCount = textureCoordinatesCount;

    if(mesh->GPUReadyBuffer) {
        prFree(mesh->GPUReadyBuffer);
        mesh->GPUReadyBuffer = NULL;
    }

    mesh->GPUReadyBuffer = prMalloc((textureCoordinatesCount + verticesCount) * sizeof(GLfloat));
    for(size_t i = 0; i < (verticesCount + textureCoordinatesCount) / 5; i++) {
        size_t vertexIndex = i * 3;
        size_t textureCoordinatesIndex = i * 2;
        mesh->GPUReadyBuffer[i * 5] = vertices[vertexIndex];
        mesh->GPUReadyBuffer[i * 5 + 1] = vertices[vertexIndex + 1];
        mesh->GPUReadyBuffer[i * 5 + 2] = vertices[vertexIndex + 2];
        mesh->GPUReadyBuffer[i * 5 + 3] = textureCoordinates[textureCoordinatesIndex];
        mesh->GPUReadyBuffer[i * 5 + 4] = textureCoordinates[textureCoordinatesIndex + 1];
    }
    mesh->GPUReadyBufferCount = verticesCount + textureCoordinatesCount;

    if(mesh->context && !mesh->VAO) {
        i_prMeshCreateOnGPUSide(mesh);
    } else if(mesh->context) {
        i_prMeshUpdateOnGPUSide(mesh);
    }
}
