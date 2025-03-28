#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <PR/memory.h>
#include <PR/error.h>

#define MAX_MESSAGESTR_LENGTH 1024

int i_prShaderProgramUniformBoilerPlate(prShaderProgramData* shaderProgram, const char* uniformName) {
    if(!shaderProgram->shaderProgramObject) {
        prLogEvent(PR_OPGL_EVENT, PR_LOG_WARN, "Shader not initialized. Aborting operation, nothing was modified");
        return -1;
    }

    int uniformLocation = shaderProgram->context->GetUniformLocation(shaderProgram->shaderProgramObject, uniformName);

    if(uniformLocation == -1) {
        prLogEvent(PR_OPGL_EVENT, PR_LOG_WARN, "No uniform with name \"%s\" found. Aborting operation, nothing was modified", uniformName);
        return -1;
    }

    return uniformLocation;
}

void i_prShaderProgramCreateOnGPU(prShaderProgramData* shaderProgram) {
    GladGLContext* context = shaderProgram->context;

    int success;
    char infoLog[MAX_MESSAGESTR_LENGTH];

    unsigned int vertexShader = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShader, 1, (const GLchar* const*)&shaderProgram->vertexShaderData, NULL);
    context->CompileShader(vertexShader);

    context->GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShader, MAX_MESSAGESTR_LENGTH, NULL, infoLog);
        context->DeleteShader(vertexShader);
        prLogEvent(PR_OPGL_EVENT, PR_LOG_WARN, "Vertex shader failed to compile. Aborting operation, nothing was modified:\n%s", infoLog);
        return;
    }

    unsigned int fragmentShader = context->CreateShader(GL_FRAGMENT_SHADER);
    context->ShaderSource(fragmentShader, 1, (const GLchar* const*)&shaderProgram->fragmentShaderData, NULL);
    context->CompileShader(fragmentShader);

    context->GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(fragmentShader, MAX_MESSAGESTR_LENGTH, NULL, infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        prLogEvent(PR_OPGL_EVENT, PR_LOG_WARN, "Fragment shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        return;
    }

    shaderProgram->shaderProgramObject = context->CreateProgram();
    context->AttachShader(shaderProgram->shaderProgramObject, vertexShader);
    context->AttachShader(shaderProgram->shaderProgramObject, fragmentShader);
    context->LinkProgram(shaderProgram->shaderProgramObject);

    context->GetProgramiv(shaderProgram->shaderProgramObject, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(shaderProgram->shaderProgramObject, MAX_MESSAGESTR_LENGTH, NULL, infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        context->DeleteProgram(shaderProgram->shaderProgramObject);
        prLogEvent(PR_OPGL_EVENT, PR_LOG_WARN, "Shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        return;
    }

    context->DeleteShader(vertexShader);
    context->DeleteShader(fragmentShader);
}

void i_prShaderProgramDestroyOnGPU(prShaderProgramData* shaderProgram) {
    shaderProgram->context->DeleteProgram(shaderProgram->shaderProgramObject);
    shaderProgram->shaderProgramObject = 0;
}
