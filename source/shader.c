#include <PR/shader.h>

#include <PR/shaderInternal.h>

#include <PR/defines.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <glad/gl.h>
#include <PR/memory.h>
#include <PR/logger.h>

#define UNIFORM_BOILERPLATE int uniformLocation = i_prShaderUniformBoilerPlate(shaderProgram, uniformName); if(uniformLocation < 0) {return;}

#define UNIFORM_BOILERPLATE_QUIET int uniformLocation = i_prShaderUniformBoilerPlate(shaderProgram, 0, uniformName); if(uniformLocation < 0) {return 0;}

prShaderData* prShaderCreate() {
    prShaderData* shaderProgram = prCalloc(1, sizeof(prShaderData));

    return shaderProgram;
}

void prShaderDestroy(prShaderData* shaderProgram) {
    if(shaderProgram->vertexShaderData) {
        prFree(shaderProgram->vertexShaderData);
        prFree(shaderProgram->fragmentShaderData);
    }
    if(shaderProgram->geometryShaderData) {
        prFree(shaderProgram->geometryShaderData);
    }

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

void prShaderUpdate(prShaderData* shaderProgram, const GLchar* vertexShader, const GLchar* fragmentShader, const GLchar* geometryShader) {
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
    if(shaderProgram->geometryShaderData) {
        prFree(shaderProgram->geometryShaderData);
        shaderProgram->geometryShaderData = NULL;
    }

    shaderProgram->vertexShaderData = prMalloc(strlen(vertexShader) + 1);
    prMemcpy(shaderProgram->vertexShaderData, (void*)vertexShader, strlen(vertexShader) + 1);

    shaderProgram->fragmentShaderData = prMalloc(strlen(fragmentShader) + 1);
    prMemcpy(shaderProgram->fragmentShaderData, (void*)fragmentShader, strlen(fragmentShader) + 1);

    if(geometryShader) {
        shaderProgram->geometryShaderData = prMalloc(strlen(geometryShader) + 1);
        prMemcpy(shaderProgram->geometryShaderData, (void*)geometryShader, strlen(geometryShader) + 1);
    }

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

// {1|2|3|4}{f|i|ui}

void prShaderSetUniform1f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform1f(shaderProgram->shaderProgramObject, uniformLocation, value);
}
void prShaderSetUniform2f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v0, GLfloat v1) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform2f(shaderProgram->shaderProgramObject, uniformLocation, v0, v1);
}
void prShaderSetUniform3f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v0, GLfloat v1, GLfloat v2) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform3f(shaderProgram->shaderProgramObject, uniformLocation, v0, v1, v2);
}
void prShaderSetUniform4f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform4f(shaderProgram->shaderProgramObject, uniformLocation, v0, v1, v2, v3);
}

void prShaderSetUniform1i(prShaderData* shaderProgram, const GLchar* uniformName, GLint value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform1i(shaderProgram->shaderProgramObject, uniformLocation, value);
}
void prShaderSetUniform2i(prShaderData* shaderProgram, const GLchar* uniformName, GLint v0, GLint v1) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform2i(shaderProgram->shaderProgramObject, uniformLocation, v0, v1);
}
void prShaderSetUniform3i(prShaderData* shaderProgram, const GLchar* uniformName, GLint v0, GLint v1, GLint v2) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform3i(shaderProgram->shaderProgramObject, uniformLocation, v0, v1, v2);
}
void prShaderSetUniform4i(prShaderData* shaderProgram, const GLchar* uniformName, GLint v0, GLint v1, GLint v2, GLint v3) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform4i(shaderProgram->shaderProgramObject, uniformLocation, v0, v1, v2, v3);
}

void prShaderSetUniform1ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform1ui(shaderProgram->shaderProgramObject, uniformLocation, value);
}
void prShaderSetUniform2ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint v0, GLuint v1) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform2ui(shaderProgram->shaderProgramObject, uniformLocation, v0, v1);
}
void prShaderSetUniform3ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint v0, GLuint v1, GLuint v2) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform3ui(shaderProgram->shaderProgramObject, uniformLocation, v0, v1, v2);
}
void prShaderSetUniform4ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint v0, GLuint v1, GLuint v2, GLuint v3) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform4ui(shaderProgram->shaderProgramObject, uniformLocation, v0, v1, v2, v3);
}

// {1|2|3|4}{f|i|ui}v

void prShaderSetUniform1fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform1fv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform2fv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform3fv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform4fv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}

void prShaderSetUniform1iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform1iv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform2iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform2iv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform3iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform3iv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform4iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform4iv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}

void prShaderSetUniform1uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform1uiv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform2uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform2uiv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform3uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform3uiv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}
void prShaderSetUniform4uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniform4uiv(shaderProgram->shaderProgramObject, uniformLocation, count, value);
}

// Matrix{1|2|3|4}fv

void prShaderSetUniformMatrix2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix2fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix3fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix4fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix2x3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix2x3fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix3x2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix3x2fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix2x4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix2x4fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix4x2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix4x2fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix3x4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix3x4fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
void prShaderSetUniformMatrix4x3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value) {
    UNIFORM_BOILERPLATE;

    shaderProgram->context->ProgramUniformMatrix4x3fv(shaderProgram->shaderProgramObject, uniformLocation, count, transpose, value);
}
