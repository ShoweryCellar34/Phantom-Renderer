#include <PR/PR.hpp>

int main(int argc, char** argv) {
    glfwInit();

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    PR::window test;
    test.windowInternal = glfwCreateWindow(600, 600, "Test", NULL, NULL);
    test.makeContext();

    while (!glfwWindowShouldClose(test.windowInternal))
    {
        test.openglContext.ClearColor(0.5f, 0.1f, 0.9f, 0.9f);
        test.openglContext.Clear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(test.windowInternal);

        glfwPollEvents();
    }
}
