#include <PR/window.h>

#include <stdio.h>
#include <PR/memory.h>
#include <PR/mesh.h>
#include <PR/texture.h>

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
    prFree(window->openglContext);
    glfwDestroyWindow(window->window);

    prFree(window);
}

void prWindowClear(GladGLContext* context) {
    context->ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
    context->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
