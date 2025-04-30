#pragma once

typedef struct prComputeShaderData prComputeShaderData;

void i_prComputeShaderCreateOnGPU(prComputeShaderData* computeShaderProgram);

void i_prComputeShaderDestroyOnGPU(prComputeShaderData* computeShaderProgram);
