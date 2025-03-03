#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

prMeshData* prMeshCreate() {
    prMeshData* mesh = malloc(sizeof(prMeshData));

    mesh->window = NULL;
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
    free(mesh);
}

void prMeshLink(prMeshData* mesh, prWindow* window) {
    if(mesh->window && mesh->GPUReadyBuffer) {
        i_prMeshDestroyOnGPUSide(mesh);
    }
    mesh->window = window;
    if(mesh->window && mesh->GPUReadyBuffer) {
        i_prMeshCreateOnGPUSide(mesh);
    }
}

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, GLuint indices[], size_t indicesCount, GLfloat textureCoordinates[], size_t textureCoordinatesCount) {
    if(mesh->vertices) {
        free(mesh->vertices);
        free(mesh->indices);
        mesh->vertices = NULL;
        mesh->indices = NULL;
    }
    if(mesh->textureCoordinates) {
        free(mesh->textureCoordinates);
        mesh->textureCoordinates = NULL;
    }

    mesh->vertices = malloc(sizeof(GLfloat) * verticesCount);
    memcpy(mesh->vertices, vertices, sizeof(GLfloat) * verticesCount);
    mesh->verticesCount = verticesCount;

    mesh->indices = malloc(sizeof(GLuint) * indicesCount);
    memcpy(mesh->indices, indices, sizeof(GLuint) * indicesCount);
    mesh->indicesCount = indicesCount;

    if(textureCoordinatesCount && textureCoordinates) {
        mesh->textureCoordinates = malloc(sizeof(GLfloat) * textureCoordinatesCount);
        memcpy(mesh->textureCoordinates, textureCoordinates, sizeof(GLfloat) * textureCoordinatesCount);
        mesh->textureCoordinatesCount = textureCoordinatesCount;
    }
    mesh->textureCoordinatesCount = textureCoordinatesCount;

    if(mesh->GPUReadyBuffer) {
        free(mesh->GPUReadyBuffer);
        mesh->GPUReadyBuffer = NULL;
    }

    mesh->GPUReadyBuffer = malloc((textureCoordinatesCount + verticesCount) * sizeof(GLfloat));
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

    if(mesh->window && !mesh->VAO) {
        i_prMeshCreateOnGPUSide(mesh);
    } else if(mesh->window) {
        i_prMeshUpdateOnGPUSide(mesh);
    }
}
