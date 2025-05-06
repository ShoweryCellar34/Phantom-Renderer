#include <PR/computeShader.h>

#include <PR/computeShaderInternal.h>

#include <PR/defines.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <PR/memory.h>
#include <PR/logger.h>

prComputeShaderData* prComputeShaderCreate() {
    prComputeShaderData* computeShaderProgram = prCalloc(1, sizeof(prComputeShaderData));

    return computeShaderProgram;
}

void prComputeShaderDestroy(prComputeShaderData* computeShaderProgram) {
    if(computeShaderProgram->computeShaderProgramObject) {
        i_prComputeShaderDestroyOnGPU(computeShaderProgram);
    }

    prFree(computeShaderProgram);
}

void prComputeShaderLinkContext(prComputeShaderData* computeShaderProgram, GladGLContext* context) {
    if(computeShaderProgram->context && computeShaderProgram->computeShaderProgramObject) {
        i_prComputeShaderDestroyOnGPU(computeShaderProgram);
    }
    computeShaderProgram->context = context;
    if(computeShaderProgram->context && computeShaderProgram->computeShaderData) {
        i_prComputeShaderCreateOnGPU(computeShaderProgram);
    }
}

void prComputeShaderUpdate(prComputeShaderData* computeShaderProgram, const GLchar* computeShader) {
    if(!computeShader) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prComputeShaderUpdate: Compute shader data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(computeShaderProgram->computeShaderData) {
        prFree(computeShaderProgram->computeShaderData);
        computeShaderProgram->computeShaderData = NULL;
    }

    computeShaderProgram->computeShaderData = prMalloc(strlen(computeShader) + 1);
    prMemcpy(computeShaderProgram->computeShaderData, (void*)computeShader, strlen(computeShader) + 1);

    if(computeShaderProgram->context && !computeShaderProgram->computeShaderData) {
        i_prComputeShaderCreateOnGPU(computeShaderProgram);
    } else if(computeShaderProgram->context) {
        i_prComputeShaderDestroyOnGPU(computeShaderProgram);
        i_prComputeShaderCreateOnGPU(computeShaderProgram);
    }
}

void prComputeShaderDispatch(prComputeShaderData* computeShaderProgram, unsigned int depth, unsigned int height, unsigned int width) {
    computeShaderProgram->context->UseProgram(computeShaderProgram->computeShaderProgramObject);
    computeShaderProgram->context->DispatchCompute(depth, height, width);
}
