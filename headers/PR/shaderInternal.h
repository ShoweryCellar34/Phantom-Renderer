#pragma once

typedef struct prShaderProgramData prShaderProgramData;

int i_prShaderProgramUniformBoilerPlate(prShaderProgramData* shaderProgram, const char* uniformName);

void i_prShaderProgramCreateOnGPU(prShaderProgramData* shaderProgram);

void i_prShaderProgramDestroyOnGPU(prShaderProgramData* shaderProgram);
