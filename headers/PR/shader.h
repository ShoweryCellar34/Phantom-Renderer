#pragma once

typedef struct GladGLContext GladGLContext;

typedef struct prShaderData {
    GladGLContext* context;
    GLchar* vertexShaderData;
    GLchar* fragmentShaderData;
    GLuint shaderProgramObject;
    int useCameraUniforms;
    int useMaterialUniforms;
} prShaderData;

prShaderData* prShaderCreate();

void prShaderDestroy(prShaderData* shaderProgram);

void prShaderLinkContext(prShaderData* shaderProgram, GladGLContext* context);

void prShaderUpdate(prShaderData* shaderProgram, int useCameraUniforms, int useMaterialUniforms, const GLchar* vertexShader, const GLchar* fragmentShader);

void prShaderUniform1i(prShaderData* shaderProgram, const GLchar* uniformName, GLint number);

void prShaderUniform1f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number);

void prShaderUniform3f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number1, GLfloat number2, GLfloat number3);

void prShaderUniformMatrix4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat* number);

int prShaderUniform1iQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLint number);

int prShaderUniform1fQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number);

int prShaderUniform3fQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat number1, GLfloat number2, GLfloat number3);

int prShaderUniformMatrix4fvQuiet(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat* number);

