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

void prShaderUpdate(prShaderData* shaderProgram, const GLchar* vertexShader, const GLchar* fragmentShader) {
    if(!vertexShader) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prShaderUpdate: Vertex shader data cannot be NULL. Aborting operation, nothing was modified");
        return;
    }
    if(!fragmentShader) {
        prLogEvent(PR_EVENT_DATA, PR_LOG_ERROR, "prShaderUpdate: Fragment shader data cannot be NULL. Aborting operation, nothing was modified");
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
        i_prShaderCreateOnGPU(shaderProgram);
    } else if(shaderProgram->context) {
        i_prShaderDestroyOnGPU(shaderProgram);
        i_prShaderCreateOnGPU(shaderProgram);
    }
}

void prShaderBind(prShaderData* shaderProgram) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject){
        return;
    }
    shaderProgram->context->UseProgram(shaderProgram->shaderProgramObject);
}

void prShaderUnbind(prShaderData* shaderProgram) {
    if(!shaderProgram || !shaderProgram->context){
        return;
    }
    shaderProgram->context->UseProgram(0);
}

GLint prShaderGetUniformLocation(prShaderData* shaderProgram, const GLchar* uniformName) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject){
        return -1;
    }
    return shaderProgram->context->GetUniformLocation(shaderProgram->shaderProgramObject, uniformName);
}

void prShaderSetAttributeLocation(prShaderData* shaderProgram, GLuint index, const GLchar* name) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject){
        return;
    }
    shaderProgram->context->BindAttribLocation(shaderProgram->shaderProgramObject, index, name);
}

void prShaderLocationSetUniform1i(prShaderData* shaderProgram, GLint location, GLint value) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject) {
        return;
    }
    shaderProgram->context->Uniform1i(location, value);
}

void prShaderLocationSetUniform1f(prShaderData* shaderProgram, GLint location, GLfloat value) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject) {
        return;
    }
    shaderProgram->context->Uniform1f(location, value);
}

void prShaderLocationSetUniform3f(prShaderData* shaderProgram, GLint location, GLfloat v1, GLfloat v2, GLfloat v3) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject) {
        return;
    }
    shaderProgram->context->Uniform3f(location, v1, v2, v3);
}

void prShaderLocationSetUniformMatrix4fv(prShaderData* shaderProgram, GLint location, const GLfloat* matrix) {
    if(!shaderProgram || !shaderProgram->context || !shaderProgram->shaderProgramObject) {
        return;
    }
    shaderProgram->context->UniformMatrix4fv(location, 1, GL_FALSE, matrix);
}

void prShaderSetUniform1i(prShaderData* shaderProgram, const GLchar* uniformName, GLint value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform1i(uniformLocation, value);
}

void prShaderSetUniform1f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform1f(uniformLocation, value);
}

void prShaderSetUniform3f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v1, GLfloat v2, GLfloat v3) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->Uniform3f(uniformLocation, v1, v2, v3);
}

void prShaderSetUniformMatrix4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat* matrix) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->UniformMatrix4fv(uniformLocation, 1, GL_FALSE, matrix);
}
