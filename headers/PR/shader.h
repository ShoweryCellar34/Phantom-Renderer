#pragma once

#include <glad/gl.h>

typedef struct prShaderProgramData {
    GladGLContext* context;
    char* vertexShaderData;
    char* fragmentShaderData;
    unsigned int shaderProgramObject;
} prShaderProgramData;

prShaderProgramData* prShaderProgramCreate();

void prShaderProgramDestroy(prShaderProgramData* shaderProgram);

void prShaderProgramLinkContext(prShaderProgramData* shaderProgram, GladGLContext* context);

void prShaderProgramUpdate(prShaderProgramData* shaderProgram, char* vertexShader, char* fragmentShader);

unsigned int prShaderGenerateDefaultProgram(GladGLContext* context);
