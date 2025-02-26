#include <PR/PR.hpp>

float vertices[] = {
//  POSITION                 COLOR                         TEXTURE COORDINATES
    0.8f,  0.8f, 0.0f,       1.0f, 1.0f, 0.0f, 0.5f,       1.0f, 1.0f,  // top right
    0.8f, -0.8f, 0.0f,       1.0f, 0.0f, 1.0f, 0.5f,       1.0f, 0.0f,  // bottom right
   -0.8f, -0.8f, 0.0f,       1.0f, 0.0f, 1.0f, 0.5f,       0.0f, 0.0f,  // bottom left
   -0.8f,  0.8f, 0.0f,       1.0f, 1.0f, 0.0f, 0.5f,       0.0f, 1.0f  // top left
};
unsigned int indices[] = {
   0, 1, 3,  // first Triangle
   1, 2, 3,  // second Triangle
};

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message);
}

int main(int argc, char** argv) {
    glfwInit();

    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    PR::window test;
    test.makeWindow("Test", 600, 600);
    test.makeContext();

    test.i_openglContext.Enable(GL_DEBUG_OUTPUT);
    test.i_openglContext.DebugMessageCallback(MessageCallback, 0);

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
        test.i_openglContext.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        test.i_openglContext.Clear(GL_COLOR_BUFFER_BIT);

        // Render
        test.drawMesh(shaderProgram, "testMesh", "testTexture");

        glfwSwapBuffers(test.i_window);

        glfwPollEvents();
    }
}
