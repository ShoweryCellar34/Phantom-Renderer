#include <PR/mesh.h>

#include <PR/meshInternal.h>

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

void prMeshInit(prMeshData* mesh) {
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
}

void prMeshLink(prMeshData* mesh, prWindow* window) {
    if(mesh->window) {
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

    mesh->GPUReadyBuffer = malloc(verticesCount * sizeof(GLfloat));
    for(size_t i = 0; i < verticesCount; i++) {
        mesh->GPUReadyBuffer[i] = vertices[i++];
        mesh->GPUReadyBuffer[i] = vertices[i++];
        mesh->GPUReadyBuffer[i] = vertices[i];
    }
    mesh->GPUReadyBufferCount = verticesCount;

    if(mesh->window && !mesh->VAO) {
        i_prMeshCreateOnGPUSide(mesh);
    } else if(mesh->window) {
        i_prMeshUpdateOnGPUSide(mesh);
    }
}
