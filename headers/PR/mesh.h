#pragma once

#include <stddef.h>
#include <glad/gl.h>
#include <cglm/cglm.h>
#include <PR/material.h>

typedef struct prTextureData prTextureData;
typedef struct prCamera prCamera;

typedef struct prMeshData {
    GladGLContext* context;
    prTextureData* texture;
    prMaterialData* material;
    GLfloat* vertices;
    GLfloat* textureCoordinates;
    GLfloat* vertexColor;
    GLfloat* vertexNormals;
    GLuint* indices;
    GLuint verticesCount, textureCoordinatesCount, vertexColorCount, vertexNormalsCount, indicesCount;
    GLuint VBO, VAO, EBO;
    GLfloat* GPUReadyBuffer;
    GLuint GPUReadyBufferCount;
    float mixRatio;
} prMeshData;

prMeshData* prMeshCreate();

void prMeshDestroy(prMeshData* mesh);

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context);

void prMeshLinkTexture(prMeshData* mesh, prTextureData* texture);

void prMeshLinkMaterial(prMeshData* mesh, prMaterialData* material);

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, 
    GLuint indices[], size_t indicesCount, 
    GLfloat textureCoordinates[], size_t textureCoordinatesCount, 
    GLfloat vertexColor[], size_t vertexColorCount, 
    GLfloat vertexNormal[], size_t vertexNormalCount);

void prMeshTextureToColorRatio(prMeshData* mesh, float mixRatio);

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera, unsigned int shaderProgram);
