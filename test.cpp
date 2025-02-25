#include <PR/PR.hpp>

float vertices[] = {
    0.8f,  0.8f, 0.0f, 1.0f, 1.0f,  // top right
    0.8f, -0.8f, 0.0f, 1.0f, 0.0f,  // bottom right
   -0.8f, -0.8f, 0.0f, 0.0f, 0.0f,  // bottom left
   -0.8f,  0.8f, 0.0f, 0.0f, 1.0f  // top left
};
unsigned int indices[] = {
   0, 1, 3,  // first Triangle
   1, 2, 3,  // second Triangle
};

int main(int argc, char** argv) {
    glfwInit();

    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    PR::window test;
    test.makeWindow("Test", 600, 600);
    test.makeContext();

    PR::meshData testMesh;
    testMesh.updateMesh(vertices, sizeof(vertices), indices, sizeof(indices), nullptr, 0);

    PR::textureData testTexture;
    testTexture.updateTexture(nullptr, 0);

    unsigned int shaderProgram = test.genDefaultShaderProgram();
    test.prepMesh(testMesh, "testMesh");
    test.prepTexture(testTexture, "testTexture");

    int width, height;
    glfwGetWindowSize(test.i_window, &width, &height);
    test.i_openglContext.Viewport(0, 0, width, height);

    while(!glfwWindowShouldClose(test.i_window)) {
        test.i_openglContext.ClearColor(0.5f, 0.1f, 0.9f, 0.9f);
        test.i_openglContext.Clear(GL_COLOR_BUFFER_BIT);

        // Render
        test.drawMesh(shaderProgram, "testMesh", "testTexture");

        glfwSwapBuffers(test.i_window);

        glfwPollEvents();
    }
}
