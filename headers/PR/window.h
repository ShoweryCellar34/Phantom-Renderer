#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

typedef struct prMeshData prMeshData;
typedef struct prTextureData prTextureData;

typedef struct prWindow {
    GLFWwindow* window;
    GladGLContext openglContext;
} prWindow;

void prWindowInit(prWindow* window, const char* title, int width, int height);

void prWindowInitContext(prWindow* window);

void prWindowDrawMesh(prWindow* window, unsigned int shaderProgram, prMeshData* mesh, prTextureData* texture);

unsigned int prWindowGenDefaultShaderProgram(prWindow* window);
