#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <glad/gl.h>
#include <PR/shader.h>
#include <PR/memory.h>
#include <PR/logger.h>

int i_prShaderUniformBoilerPlate(prShaderData* shaderProgram, int logMissing, const char* uniformName) {
    if(!shaderProgram->shaderProgramObject) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderUniformBoilerPlate: Shader not initialized. Aborting operation, nothing was modified");
        return -1;
    }

    shaderProgram->context->UseProgram(shaderProgram->shaderProgramObject);
    int uniformLocation = shaderProgram->context->GetUniformLocation(shaderProgram->shaderProgramObject, uniformName);

    if(uniformLocation == -1) {
        // To verbose because this will happen whenever the uniform is set, and can be an issue when a uniform set by prMeshDraw() is not found in the shader.
        if(logMissing != 0) {
            prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderUniformBoilerPlate: No uniform with name \"%s\" found. Aborting operation, nothing was modified", uniformName);
        }
        return -1;
    }

    return uniformLocation;
}

void i_prShaderCreateOnGPU(prShaderData* shaderProgram) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prShaderCreateOnGPU: Creating shader program");

    GladGLContext* context = shaderProgram->context;

    int success;
    char infoLog[PR_MAXSTR_LEN];

    unsigned int vertexShader = context->CreateShader(GL_VERTEX_SHADER);
    context->ShaderSource(vertexShader, 1, (const GLchar* const*)&shaderProgram->vertexShaderData, NULL);
    context->CompileShader(vertexShader);

    context->GetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(vertexShader, PR_MAXSTR_LEN, NULL, infoLog);
        context->DeleteShader(vertexShader);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderCreateOnGPU: Vertex shader failed to compile. Aborting operation, nothing was modified:\n%s", infoLog);
        return;
    }

    unsigned int fragmentShader = context->CreateShader(GL_FRAGMENT_SHADER);
    context->ShaderSource(fragmentShader, 1, (const GLchar* const*)&shaderProgram->fragmentShaderData, NULL);
    context->CompileShader(fragmentShader);

    context->GetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        context->GetShaderInfoLog(fragmentShader, PR_MAXSTR_LEN, NULL, infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderCreateOnGPU: Fragment shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
        return;
    }

    shaderProgram->shaderProgramObject = context->CreateProgram();
    context->AttachShader(shaderProgram->shaderProgramObject, vertexShader);
    context->AttachShader(shaderProgram->shaderProgramObject, fragmentShader);
    context->LinkProgram(shaderProgram->shaderProgramObject);

    context->GetProgramiv(shaderProgram->shaderProgramObject, GL_LINK_STATUS, &success);
    if(!success) {
        context->GetProgramInfoLog(shaderProgram->shaderProgramObject, PR_MAXSTR_LEN, NULL, infoLog);
        context->DeleteShader(vertexShader);
        context->DeleteShader(fragmentShader);
        context->DeleteProgram(shaderProgram->shaderProgramObject);
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderCreateOnGPU: Shader program failed to link. Aborting operation, nothing was modified: %s", infoLog);
        return;
    }

    context->DeleteShader(vertexShader);
    context->DeleteShader(fragmentShader);
}

void i_prShaderDestroyOnGPU(prShaderData* shaderProgram) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prShaderDestroyOnGPU: Destroying shader program");

    shaderProgram->context->DeleteProgram(shaderProgram->shaderProgramObject);
    shaderProgram->shaderProgramObject = 0;
}
