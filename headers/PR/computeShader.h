#pragma once

typedef struct GladGLContext GladGLContext;

typedef struct prComputeShaderData {
    GladGLContext* context;
    char* computeShaderData;
    unsigned int computeShaderProgramObject;
} prComputeShaderData;

prComputeShaderData* prComputeShaderCreate();

void prComputeShaderDestroy(prComputeShaderData* computeShaderProgram);

void prComputeShaderLinkContext(prComputeShaderData* computeShaderProgram, GladGLContext* context);

void prComputeShaderUpdate(prComputeShaderData* computeShaderProgram, const char* computeShader);

void prComputeShaderDispatch(prComputeShaderData* computeShaderProgram, unsigned int depth, unsigned int height, unsigned int width);
