#include <PR/shader.h>

#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <PR/memory.h>
#include <PR/error.h>

#define UNIFORM_BOILERPLATE int uniformLocation = i_prShaderProgramUniformBoilerPlate(shaderProgram, uniformName); if(uniformLocation < 0) {return;}

prShaderProgramData* prShaderProgramCreate() {
    prShaderProgramData* shaderProgram = prMalloc(sizeof(prShaderProgramData));

    shaderProgram->context = NULL;
    shaderProgram->vertexShaderData = NULL;
    shaderProgram->fragmentShaderData = NULL;
    shaderProgram->shaderProgramObject = 0;

    return shaderProgram;
}

void prShaderProgramDestroy(prShaderProgramData* shaderProgram) {
    if(shaderProgram->shaderProgramObject) {
        i_prShaderProgramDestroyOnGPU(shaderProgram);
    }

    prFree(shaderProgram);
}

void prShaderProgramLinkContext(prShaderProgramData* shaderProgram, GladGLContext* context) {
    if(shaderProgram->context && shaderProgram->shaderProgramObject) {
        i_prShaderProgramDestroyOnGPU(shaderProgram);
    }
    shaderProgram->context = context;
    if(shaderProgram->context && shaderProgram->vertexShaderData) {
        i_prShaderProgramCreateOnGPU(shaderProgram);
    }
}

void prShaderProgramUpdate(prShaderProgramData* shaderProgram, const char* vertexShader, const char* fragmentShader) {
    if(!vertexShader) {
        prLogEvent(PR_DATA_EVENT, PR_LOG_EROR, "Vertex shader data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }
    if(!fragmentShader) {
        prLogEvent(PR_DATA_EVENT, PR_LOG_EROR, "Fragment shader data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    if(shaderProgram->vertexShaderData) {
        prFree(shaderProgram->vertexShaderData);
        prFree(shaderProgram->fragmentShaderData);
        shaderProgram->vertexShaderData = NULL;
        shaderProgram->fragmentShaderData = NULL;
    }

    shaderProgram->vertexShaderData = prMalloc(strlen(vertexShader) + 1);
    prMemcpy(shaderProgram->vertexShaderData, (void*)vertexShader, strlen(vertexShader) + 1);

    shaderProgram->fragmentShaderData = prMalloc(strlen(fragmentShader) + 1);
    prMemcpy(shaderProgram->fragmentShaderData, (void*)fragmentShader, strlen(fragmentShader) + 1);

    if(shaderProgram->context && !shaderProgram->shaderProgramObject) {
        i_prShaderProgramCreateOnGPU(shaderProgram);
    } else if(shaderProgram->context) {
        i_prShaderProgramDestroyOnGPU(shaderProgram);
        i_prShaderProgramCreateOnGPU(shaderProgram);
    }
}

void prShaderProgramUniform1f(prShaderProgramData* shaderProgram, const char* uniformName, float number) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform1f(uniformLocation, number);
}

void prShaderProgramUniform3f(prShaderProgramData* shaderProgram, const char* uniformName, float number1, float number2, float number3) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform3f(uniformLocation, number1, number2, number3);
}

void prShaderProgramUniformMatrix4fv(prShaderProgramData* shaderProgram, const char* uniformName, float* number) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->UniformMatrix4fv(uniformLocation, 1, GL_FALSE, number);
}
