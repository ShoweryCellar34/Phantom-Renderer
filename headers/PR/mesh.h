#pragma once

#include <PR/defines.h>

#include <glad/gl.h>
#include <cglm/cglm.h>

typedef struct prMaterialData prMaterialData;
typedef struct prCamera prCamera;
typedef struct prShaderProgramData prShaderProgramData;

typedef struct prMeshData {
    GladGLContext* context;
    prMaterialData* material;
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

void prMeshLinkContext(prMeshData* mesh, GladGLContext* context);

void prMeshLinkMaterial(prMeshData* mesh, prMaterialData* material);

void prMeshUpdate(prMeshData* mesh, GLfloat vertices[], size_t verticesCount, 
    GLuint indices[], size_t indicesCount, 
    GLfloat textureCoordinates[], size_t textureCoordinatesCount);

void prMeshDraw(prMeshData* mesh, mat4 translation, prCamera* camera, prShaderProgramData* shaderProgram);
