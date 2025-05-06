#pragma once

#include <glad/gl.h>

typedef struct prComputeShaderData {
    GladGLContext* context;
    GLchar* computeShaderData;
    GLuint computeShaderProgramObject;
} prComputeShaderData;

prComputeShaderData* prComputeShaderCreate();

void prComputeShaderDestroy(prComputeShaderData* computeShaderProgram);

void prComputeShaderLinkContext(prComputeShaderData* computeShaderProgram, GladGLContext* context);

void prComputeShaderUpdate(prComputeShaderData* computeShaderProgram, const GLchar* computeShader);

void prComputeShaderDispatch(prComputeShaderData* computeShaderProgram, unsigned int depth, unsigned int height, unsigned int width);
