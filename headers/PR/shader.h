#pragma once

#include <glad/gl.h>

typedef struct prShaderData {
    GladGLContext* context;
    GLchar* vertexShaderData;
    GLchar* fragmentShaderData;
    GLchar* geometryShaderData;
    GLuint shaderProgramObject;
} prShaderData;

prShaderData* prShaderCreate();
void prShaderDestroy(prShaderData* shaderProgram);
void prShaderLinkContext(prShaderData* shaderProgram, GladGLContext* context);
void prShaderUpdate(prShaderData* shaderProgram, const GLchar* vertexShader, const GLchar* fragmentShader, const GLchar* geometryShader);

void prShaderBind(prShaderData* shaderProgram);
void prShaderUnbind(prShaderData* shaderProgram);
GLint prShaderGetUniformLocation(prShaderData* shaderProgram, const GLchar* uniformName);
void prShaderSetAttributeLocation(prShaderData* shaderProgram, GLuint index, const GLchar* name);

void prShaderLocationSetUniform1i(prShaderData* shaderProgram, GLint location, GLint value);
void prShaderLocationSetUniform1f(prShaderData* shaderProgram, GLint location, GLfloat value);
void prShaderLocationSetUniform2f(prShaderData* shaderProgram, GLint location, GLfloat v1, GLfloat v2);
void prShaderLocationSetUniform3f(prShaderData* shaderProgram, GLint location, GLfloat v1, GLfloat v2, GLfloat v3);
void prShaderLocationSetUniformMatrix4fv(prShaderData* shaderProgram, GLint location, const GLfloat* matrix);

void prShaderSetUniform1i(prShaderData* shaderProgram, const GLchar* uniformName, GLint value);
void prShaderSetUniform1f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat value);
void prShaderSetUniform2f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v1, GLfloat v2);
void prShaderSetUniform3f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v1, GLfloat v2, GLfloat v3);
void prShaderSetUniformMatrix4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat* matrix);
