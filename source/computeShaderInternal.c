#include <PR/computeShaderInternal.h>

#include <PR/defines.h>

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <glad/gl.h>
#include <PR/computeShader.h>
#include <PR/memory.h>
#include <PR/logger.h>

void i_prComputeShaderCreateOnGPU(prComputeShaderData* computeShaderProgram) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prComputeShaderCreateOnGPU: Creating shader program");

    GladGLContext* context = computeShaderProgram->context;

    GLint success;
    GLchar infoLog[PR_MAXSTR_LEN];

    GLuint computeShader = context->CreateShader(GL_COMPUTE_SHADER);
    context->ShaderSource(computeShader, 1, (const GLchar* const*)&computeShaderProgram->computeShaderData, NULL);
    context->CompileShader(computeShader);

    context->GetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(computeShader, PR_MAXSTR_LEN, NULL, infoLog);
        context->DeleteShader(computeShader);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prComputeShaderCreateOnGPU: Compute shader failed to compile. Aborting operation, nothing was modified:\n%s", infoLog);
        return;
    }

    computeShaderProgram->computeShaderProgramObject = context->CreateProgram();
    context->AttachShader(computeShaderProgram->computeShaderProgramObject, computeShader);
    context->LinkProgram(computeShaderProgram->computeShaderProgramObject);

    context->GetProgramiv(computeShaderProgram->computeShaderProgramObject, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(computeShaderProgram->computeShaderProgramObject, PR_MAXSTR_LEN, NULL, infoLog);
        context->DeleteShader(computeShader);
        context->DeleteProgram(computeShaderProgram->computeShaderProgramObject);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prComputeShaderCreateOnGPU: Compute shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        return;
    }

    context->DeleteShader(computeShader);
}

void i_prComputeShaderDestroyOnGPU(prComputeShaderData* computeShaderProgram) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prComputeShaderDestroyOnGPU: Destroying shader program");

    computeShaderProgram->context->DeleteProgram(computeShaderProgram->computeShaderProgramObject);
    computeShaderProgram->computeShaderProgramObject = 0;
}
