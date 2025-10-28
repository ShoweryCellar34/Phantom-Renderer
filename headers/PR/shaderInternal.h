#ifndef PR_SHADER_INTERNAL_H
#define PR_SHADER_INTERNAL_H

#include <glad/gl.h>

typedef struct prShaderData prShaderData;

int i_prShaderUniformBoilerPlate(prShaderData* shaderProgram, const GLchar* uniformName);

void i_prShaderCreateOnGPU(prShaderData* shaderProgram);

void i_prShaderDestroyOnGPU(prShaderData* shaderProgram);

#endif // PR_SHADER_INTERNAL_H
