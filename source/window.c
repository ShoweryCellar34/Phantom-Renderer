#include <PR/window.h>

#include <stdio.h>
#include <PR/memory.h>
#include <PR/mesh.h>
#include <PR/texture.h>

prWindow* prWindowCreate(const char* title, int width, int height) {
    prWindow* window = prMalloc(sizeof(prWindow));

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

void prWindowDrawMesh(prWindow* window, unsigned int shaderProgram, prMeshData* mesh, prTextureData* texture) {
    window->openglContext->UseProgram(shaderProgram);

    window->openglContext->BindVertexArray(mesh->VAO);

    window->openglContext->ActiveTexture(GL_TEXTURE0);
    window->openglContext->BindTexture(GL_TEXTURE_2D, texture->TBO);

    window->openglContext->DrawElements(GL_TRIANGLES, mesh->indicesCount, GL_UNSIGNED_INT, 0);

    window->openglContext->BindVertexArray(0);
}
