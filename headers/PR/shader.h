#pragma once

typedef struct GladGLContext GladGLContext;

typedef struct prShaderData {
    GladGLContext* context;
    char* vertexShaderData;
    char* fragmentShaderData;
    unsigned int shaderProgramObject;
    int useCameraUniforms;
    int useMaterialUniforms;
} prShaderData;

prShaderData* prShaderCreate();

void prShaderDestroy(prShaderData* shaderProgram);

void prShaderLinkContext(prShaderData* shaderProgram, GladGLContext* context);

void prShaderUpdate(prShaderData* shaderProgram, int useCameraUniforms, int useMaterialUniforms, const char* vertexShader, const char* fragmentShader);

void prShaderUniform1i(prShaderData* shaderProgram, const char* uniformName, int number);

void prShaderUniform1f(prShaderData* shaderProgram, const char* uniformName, float number);

void prShaderUniform3f(prShaderData* shaderProgram, const char* uniformName, float number1, float number2, float number3);

void prShaderUniformMatrix4fv(prShaderData* shaderProgram, const char* uniformName, float* number);

int prShaderUniform1iQuiet(prShaderData* shaderProgram, const char* uniformName, int number);

int prShaderUniform1fQuiet(prShaderData* shaderProgram, const char* uniformName, float number);

int prShaderUniform3fQuiet(prShaderData* shaderProgram, const char* uniformName, float number1, float number2, float number3);

int prShaderUniformMatrix4fvQuiet(prShaderData* shaderProgram, const char* uniformName, float* number);

