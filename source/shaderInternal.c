#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <glad/gl.h>
#include <PR/shader.h>
#include <PR/memory.h>
#include <PR/logger.h>

int i_prShaderUniformBoilerPlate(prShaderData* shaderProgram, const GLchar* uniformName) {
    if(!shaderProgram->shaderProgramObject) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderUniformBoilerPlate: Shader not initialized. Aborting operation, nothing was modified");
        return -1;
    }

    shaderProgram->context->UseProgram(shaderProgram->shaderProgramObject);
    GLint uniformLocation = shaderProgram->context->GetUniformLocation(shaderProgram->shaderProgramObject, uniformName);

    if(uniformLocation == -1) {
        prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderUniformBoilerPlate: No uniform with name \"%s\" found. Aborting operation, nothing was modified", uniformName);
        return -1;
    }

    return uniformLocation;
}

void i_prShaderCreateOnGPU(prShaderData* shaderProgram) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_INFO, "i_prShaderCreateOnGPU: Creating shader program");

    GladGLContext* context = shaderProgram->context;

    GLint success;
    GLchar infoLog[PR_MAXSTR_LEN];

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

    unsigned int geometryShader = 0;
    if(shaderProgram->geometryShaderData) {
        geometryShader = context->CreateShader(GL_GEOMETRY_SHADER);
        context->ShaderSource(geometryShader, 1, (const GLchar* const*)&shaderProgram->geometryShaderData, NULL);
        context->CompileShader(geometryShader);

        context->GetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            context->GetShaderInfoLog(geometryShader, PR_MAXSTR_LEN, NULL, infoLog);
            context->DeleteShader(vertexShader);
            context->DeleteShader(fragmentShader);
            context->DeleteShader(geometryShader);
            prLogEvent(PR_EVENT_OPENGL, PR_LOG_WARNING, "i_prShaderCreateOnGPU: Geometry shader failed to compile. Aborting operation, nothing was modified: %s", infoLog);
            return;
        }
    }

    shaderProgram->shaderProgramObject = context->CreateProgram();
    context->AttachShader(shaderProgram->shaderProgramObject, vertexShader);
    context->AttachShader(shaderProgram->shaderProgramObject, fragmentShader);
    if(geometryShader) {
        context->AttachShader(shaderProgram->shaderProgramObject, geometryShader);
    }
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
    if(geometryShader) {
        context->DeleteShader(geometryShader);
    }
}

void i_prShaderDestroyOnGPU(prShaderData* shaderProgram) {
    prLogEvent(PR_EVENT_OPENGL, PR_LOG_TRACE, "i_prShaderDestroyOnGPU: Destroying shader program");

    shaderProgram->context->DeleteProgram(shaderProgram->shaderProgramObject);
    shaderProgram->shaderProgramObject = 0;
}
