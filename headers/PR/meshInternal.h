#pragma once

#include <glad/gl.h>

typedef struct prMeshData prMeshData;
typedef struct prCamera prCamera;
typedef struct prShaderData prShaderData;

void i_prMeshComputeGPUReadyBuffer(prMeshData* mesh);

void i_prMeshCreateOnGPU(prMeshData* mesh);

void i_prMeshUpdateOnGPU(prMeshData* mesh);

void i_prMeshDestroyOnGPU(prMeshData* mesh);

void i_prMeshDrawIndicesOnGPU(prMeshData* mesh);

void i_prMeshDrawIndicesInstancesOnGPU(prMeshData* mesh, GLsizei count);

void i_prMeshDrawOnGPU(prMeshData* mesh, GLsizei verticesCount);

void i_prMeshDrawInstancesOnGPU(prMeshData* mesh, GLsizei verticesCount, GLsizei count);
