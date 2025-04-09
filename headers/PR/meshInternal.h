#pragma once

#include <cglm/cglm.h>

typedef struct prMeshData prMeshData;
typedef struct prCamera prCamera;
typedef struct prShaderData prShaderData;

void i_prMeshComputeGPUReadyBuffer(prMeshData* mesh);

void i_prMeshCreateOnGPU(prMeshData* mesh);

void i_prMeshUpdateOnGPU(prMeshData* mesh);

void i_prMeshDestroyOnGPU(prMeshData* mesh);

void i_prMeshDrawOnGPU(prMeshData* mesh, mat4 translation, prCamera* camera, prShaderData* shaderProgram);
