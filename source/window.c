#include <PR/window.h>

#include <stdio.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <PR/memory.h>

prWindow* prWindowCreate(const char* title, int width, int height) {
    prWindow* window = prMalloc(sizeof(prWindow));

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window->window = glfwCreateWindow(width, height, title, NULL, NULL);

    return window;
}

void prWindowInitContext(prWindow* window) {
    glfwMakeContextCurrent(window->window);
    window->openglContext = prMalloc(sizeof(GladGLContext));
    gladLoadGLContext(window->openglContext, (GLADloadfunc)glfwGetProcAddress);
}

void prWindowDestroy(prWindow* window) {
    if(window->openglContext) {
        prFree(window->openglContext);
    }
    glfwDestroyWindow(window->window);

    prFree(window);
}
