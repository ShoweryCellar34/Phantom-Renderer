#pragma once

#include <glad/gl.h>

typedef struct prShaderData prShaderData;

int i_prShaderUniformBoilerPlate(prShaderData* shaderProgram, const GLchar* uniformName);

void i_prShaderCreateOnGPU(prShaderData* shaderProgram);

void i_prShaderDestroyOnGPU(prShaderData* shaderProgram);
