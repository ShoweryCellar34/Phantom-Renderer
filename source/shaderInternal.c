#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <stddef.h>
#include <stdio.h>
#include <PR/error.h>

#define INFO_MESSAGE_LENGTH 512
#define FAIL_MESSAGE_LENGTH 786

int i_prShaderProgramUniformBoilerPlate(prShaderProgramData* shaderProgram, const char* uniformName) {
    if(!shaderProgram->shaderProgramObject) {
        prError(PR_GL_ERROR, "Shader not initialized. Aborting operation, nothing was modified");
        return -1;
    }

    int uniformLocation = shaderProgram->context->GetUniformLocation(shaderProgram->shaderProgramObject, uniformName);

    if(uniformLocation < 0) {
        prError(PR_GL_ERROR, "No uniform found. Aborting operation, nothing was modified");
        return -1;
    }

    return uniformLocation;
}

void i_prShaderProgramCreateOnGPU(prShaderProgramData* shaderProgram) {
    GladGLContext* context = shaderProgram->context;

    int success;
    char infoLog[INFO_MESSAGE_LENGTH];
    char failMessage[FAIL_MESSAGE_LENGTH];

    unsigned int vertexShader = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShader, 1, (const GLchar* const*)&shaderProgram->vertexShaderData, NULL);
    context->CompileShader(vertexShader);

    context->GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShader, INFO_MESSAGE_LENGTH, NULL, infoLog);
        snprintf(failMessage, FAIL_MESSAGE_LENGTH, "Vertex shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        prError(PR_GL_ERROR, failMessage);
        return;
    }

    unsigned int fragmentShader = context->CreateShader(GL_FRAGMENT_SHADER);
    context->ShaderSource(fragmentShader, 1, (const GLchar* const*)&shaderProgram->fragmentShaderData, NULL);
    context->CompileShader(fragmentShader);

    context->GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(fragmentShader, INFO_MESSAGE_LENGTH, NULL, infoLog);
        snprintf(failMessage, FAIL_MESSAGE_LENGTH, "Fragment shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        prError(PR_GL_ERROR, failMessage);
        return;
    }

    shaderProgram->shaderProgramObject = context->CreateProgram();
    context->AttachShader(shaderProgram->shaderProgramObject, vertexShader);
    context->AttachShader(shaderProgram->shaderProgramObject, fragmentShader);
    context->LinkProgram(shaderProgram->shaderProgramObject);

    context->GetProgramiv(shaderProgram->shaderProgramObject, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(shaderProgram->shaderProgramObject, INFO_MESSAGE_LENGTH, NULL, infoLog);
        snprintf(failMessage, FAIL_MESSAGE_LENGTH, "Shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        context->DeleteProgram(shaderProgram->shaderProgramObject);
        prError(PR_GL_ERROR, failMessage);
        return;
    }

    context->DeleteShader(vertexShader);
    context->DeleteShader(fragmentShader);
}

void i_prShaderProgramDestroyOnGPU(prShaderProgramData* shaderProgram) {
    shaderProgram->context->DeleteProgram(shaderProgram->shaderProgramObject);
    shaderProgram->shaderProgramObject = 0;
}
