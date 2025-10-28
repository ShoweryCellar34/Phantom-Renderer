#ifndef PR_COMPUTE_SHADER_INTERNAL_H
#define PR_COMPUTE_SHADER_INTERNAL_H

typedef struct prComputeShaderData prComputeShaderData;

void i_prComputeShaderCreateOnGPU(prComputeShaderData* computeShaderProgram);

void i_prComputeShaderDestroyOnGPU(prComputeShaderData* computeShaderProgram);

#endif // PR_COMPUTE_SHADER_INTERNAL_H
