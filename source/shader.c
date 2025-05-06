#include <PR/shader.h>

#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <glad/gl.h>
#include <PR/memory.h>
#include <PR/logger.h>

#define UNIFORM_BOILERPLATE int uniformLocation = i_prShaderUniformBoilerPlate(shaderProgram, 1, uniformName); if(uniformLocation < 0) {return;}

#define UNIFORM_BOILERPLATE_QUIET int uniformLocation = i_prShaderUniformBoilerPlate(shaderProgram, 0, uniformName); if(uniformLocation < 0) {return 0;}

prShaderData* prShaderCreate() {
    prShaderData* shaderProgram = prCalloc(1, sizeof(prShaderData));

    return shaderProgram;
}

void prShaderDestroy(prShaderData* shaderProgram) {
    if(shaderProgram->shaderProgramObject) {
        i_prShaderDestroyOnGPU(shaderProgram);
    }

    prFree(shaderProgram);
}

void prShaderLinkContext(prShaderData* shaderProgram, GladGLContext* context) {
    if(shaderProgram->context && shaderProgram->shaderProgramObject) {
        i_prShaderDestroyOnGPU(shaderProgram);
    }
    shaderProgram->context = context;
    if(shaderProgram->context && shaderProgram->vertexShaderData) {
        i_prShaderCreateOnGPU(shaderProgram);
    }
}

void prShaderUpdate(prShaderData* shaderProgram, int useCameraUniforms, int useMaterialUniforms, const GLchar* vertexShader, const GLchar* fragmentShader) {
    if(!vertexShader) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prShaderUpdate: Vertex shader data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }
    if(!fragmentShader) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prShaderUpdate: Fragment shader data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }

    shaderProgram->useCameraUniforms = useCameraUniforms;
    shaderProgram->useMaterialUniforms = useMaterialUniforms;

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
        i_prShaderCreateOnGPU(shaderProgram);
    } else if(shaderProgram->context) {
        i_prShaderDestroyOnGPU(shaderProgram);
        i_prShaderCreateOnGPU(shaderProgram);
    }
}

void prShaderUniform1i(prShaderData* shaderProgram, const GLchar* uniformName, GLint number) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform1i(uniformLocation, number);
}

void prShaderUniform1f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform1f(uniformLocation, number);
}

void prShaderUniform3f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number1, GLfloat number2, GLfloat number3) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform3f(uniformLocation, number1, number2, number3);
}

void prShaderUniformMatrix4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat* number) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->UniformMatrix4fv(uniformLocation, 1, GL_FALSE, number);
}

int prShaderUniform1fQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number) {
    UNIFORM_BOILERPLATE_QUIET;

    shaderProgram->context->Uniform1f(uniformLocation, number);

    return 1;
}

int prShaderUniform1iQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLint number) {
    UNIFORM_BOILERPLATE_QUIET;

    shaderProgram->context->Uniform1i(uniformLocation, number);

    return 1;
}

int prShaderUniform3fQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number1, GLfloat number2, GLfloat number3) {
    UNIFORM_BOILERPLATE_QUIET;

    shaderProgram->context->Uniform3f(uniformLocation, number1, number2, number3);

    return 1;
}

int prShaderUniformMatrix4fvQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat* number) {
    UNIFORM_BOILERPLATE_QUIET;

    shaderProgram->context->UniformMatrix4fv(uniformLocation, 1, GL_FALSE, number);

    return 1;
}
