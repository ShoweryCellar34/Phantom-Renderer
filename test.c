#include <PR/PR.h>

#include <stdio.h>

float vertices[] = {
    0.8f,  0.8f, 0.0f,  // top right
    0.8f, -0.8f, 0.0f,  // bottom right
   -0.8f, -0.8f, 0.0f,  // bottom left
   -0.8f,  0.8f, 0.0f // top left
};
float textureCoordinates[] = {
    1.0f,  1.0f,
    1.0f, -1.0f,
   -1.0f, -1.0f,
   -1.0f,  1.0f
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
    prWindow test;
    prWindowInit(&test, "Test", 600, 600);
    prWindowInitContext(&test);

    test.openglContext.Enable(GL_DEBUG_OUTPUT);
    test.openglContext.DebugMessageCallback(MessageCallback, 0);

    prMeshData testMesh;
    prMeshInit(&testMesh);
    prMeshLink(&testMesh, &test);
    prMeshUpdate(&testMesh, vertices, 12, indices, 6, textureCoordinates, 8);

    for(size_t i = 0; i < testMesh.verticesCount; i++) {
        printf("%f\n", testMesh.vertices[i]);
    }

    prTextureData testTexture;
    prTextureInit(&testTexture);
    prTextureLink(&testTexture, &test);
    prTextureUpdate(&testTexture, NULL, 0);

    unsigned int shaderProgram = prWindowGenDefaultShaderProgram(&test);

    int width, height;
    glfwGetWindowSize(test.window, &width, &height);
    test.openglContext.Viewport(0, 0, width, height);

    while(!glfwWindowShouldClose(test.window)) {
        test.openglContext.ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        test.openglContext.Clear(GL_COLOR_BUFFER_BIT);

        prWindowDrawMesh(&test, shaderProgram, &testMesh, &testTexture);

        glfwSwapBuffers(test.window);

        glfwPollEvents();
    }
}
