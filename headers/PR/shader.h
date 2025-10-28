#ifndef PR_SHADER_H
#define PR_SHADER_H

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

// {1|2|3|4}{f|i|ui}

void prShaderSetUniform1f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat value);
void prShaderSetUniform2f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v0, GLfloat v1);
void prShaderSetUniform3f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v0, GLfloat v1, GLfloat v2);
void prShaderSetUniform4f(prShaderData* shaderProgram, const GLchar* uniformName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

void prShaderSetUniform1i(prShaderData* shaderProgram, const GLchar* uniformName, GLint value);
void prShaderSetUniform2i(prShaderData* shaderProgram, const GLchar* uniformName, GLint v0, GLint v1);
void prShaderSetUniform3i(prShaderData* shaderProgram, const GLchar* uniformName, GLint v0, GLint v1, GLint v2);
void prShaderSetUniform4i(prShaderData* shaderProgram, const GLchar* uniformName, GLint v0, GLint v1, GLint v2, GLint v3);

void prShaderSetUniform1ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint value);
void prShaderSetUniform2ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint v0, GLuint v1);
void prShaderSetUniform3ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint v0, GLuint v1, GLuint v2);
void prShaderSetUniform4ui(prShaderData* shaderProgram, const GLchar* uniformName, GLuint v0, GLuint v1, GLuint v2, GLuint v3);

// {1|2|3|4}{f|i|ui}v

void prShaderSetUniform1fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value);
void prShaderSetUniform2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value);
void prShaderSetUniform3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value);
void prShaderSetUniform4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLfloat* value);

void prShaderSetUniform1iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value);
void prShaderSetUniform2iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value);
void prShaderSetUniform3iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value);
void prShaderSetUniform4iv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLint* value);

void prShaderSetUniform1uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value);
void prShaderSetUniform2uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value);
void prShaderSetUniform3uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value);
void prShaderSetUniform4uiv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, const GLuint* value);

// Matrix{1|2|3|4}fv

void prShaderSetUniformMatrix2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix2x3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix3x2fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix2x4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix4x3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix3x4fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);
void prShaderSetUniformMatrix4x3fv(prShaderData* shaderProgram, const GLchar* uniformName, GLsizei count, GLboolean transpose, const GLfloat* value);

#endif // PR_SHADER_H
