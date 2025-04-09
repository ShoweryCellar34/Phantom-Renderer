#pragma once

typedef struct prShaderData prShaderData;

int i_prShaderUniformBoilerPlate(prShaderData* shaderProgram, const char* uniformName);

void i_prShaderCreateOnGPU(prShaderData* shaderProgram);

void i_prShaderDestroyOnGPU(prShaderData* shaderProgram);
