#pragma once

typedef struct GLFWwindow GLFWwindow;
typedef struct GladGLContext GladGLContext;
typedef struct prMeshData prMeshData;
typedef struct prTextureData prTextureData;

typedef struct prWindow {
    GLFWwindow* window;
    GladGLContext* openglContext;
} prWindow;

prWindow* prWindowCreate(const char* title, int width, int height);

void prWindowDestroy(prWindow* window);

void prWindowInitContext(prWindow* window);
