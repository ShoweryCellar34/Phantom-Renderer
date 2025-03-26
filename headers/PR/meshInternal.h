#pragma once

#include <PR/mesh.h>

void i_prMeshComputeGPUReadyBuffer(prMeshData* mesh);

void i_prMeshCreateOnGPU(prMeshData* mesh);

void i_prMeshUpdateOnGPU(prMeshData* mesh);

void i_prMeshDestroyOnGPU(prMeshData* mesh);
