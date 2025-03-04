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
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
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
    context->Clear(GL_COLOR_BUFFER_BIT);
}

void prWindowDrawMesh(GladGLContext* context, unsigned int shaderProgram, prMeshData* mesh, prTextureData* texture) {
    context->UseProgram(shaderProgram);

    context->BindVertexArray(mesh->VAO);

    context->ActiveTexture(GL_TEXTURE0);
    context->BindTexture(GL_TEXTURE_2D, texture->TBO);

    context->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    context->BindVertexArray(0);
}
