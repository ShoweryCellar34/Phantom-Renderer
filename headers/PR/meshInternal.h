#pragma once

#include <PR/mesh.h>

void i_prComputeGPUReadyBuffer(prMeshData* mesh);

void i_prMeshCreateOnGPUSide(prMeshData* mesh);

void i_prMeshUpdateOnGPUSide(prMeshData* mesh);

void i_prMeshDestroyOnGPUSide(prMeshData* mesh);
