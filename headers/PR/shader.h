#pragma once

typedef struct GladGLContext GladGLContext;

typedef struct prShaderProgramData {
    GladGLContext* context;
    char* vertexShaderData;
    char* fragmentShaderData;
    unsigned int shaderProgramObject;
} prShaderProgramData;

prShaderProgramData* prShaderProgramCreate();

void prShaderProgramDestroy(prShaderProgramData* shaderProgram);

void prShaderProgramLinkContext(prShaderProgramData* shaderProgram, GladGLContext* context);

void prShaderProgramUpdate(prShaderProgramData* shaderProgram, const char* vertexShader, const char* fragmentShader);

void prShaderProgramUniform1i(prShaderProgramData* shaderProgram, const char* uniformName, int number);

void prShaderProgramUniform1f(prShaderProgramData* shaderProgram, const char* uniformName, float number);

void prShaderProgramUniform3f(prShaderProgramData* shaderProgram, const char* uniformName, float number1, float number2, float number3);

void prShaderProgramUniformMatrix4fv(prShaderProgramData* shaderProgram, const char* uniformName, float* number);
