#include <PR/PR.h>

#include <stdio.h>
#include <PR/memory.h>

float vertices[] = {
    0.8f,  0.8f, 0.0f,  // top right
    0.8f, -0.8f, 0.0f,  // bottom right
   -0.8f, -0.8f, 0.0f,  // bottom left
   -0.8f,  0.8f, 0.0f // top left
};

float vertices2[] = {
    1.0f,  1.0f, 0.0f,  // top right
    1.0f, -0.2f, 0.0f,  // bottom right
   -0.2f, -0.2f, 0.0f,  // bottom left
   -0.2f,  1.0f, 0.0f // top left
};

float textureCoordinates[] = {
    1.0f,  1.0f,
    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  1.0f
};
unsigned int indices[] = {
   0, 1, 3,  // first Triangle
   1, 2, 3,  // second Triangle
};

// void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
//     fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
//            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
//             type, severity, message);
// }

int main(int argc, char** argv) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    prWindow* test = prWindowCreate("Test", 600, 600);
    prWindowInitContext(test);

    prEnableBlending(test->openglContext);
    prEnableImageFlip();

    // test.openglContext.Enable(GL_DEBUG_OUTPUT);
    // test.openglContext.DebugMessageCallback(MessageCallback, 0);
    
    unsigned int shaderProgram = prWindowGenDefaultShaderProgram(test);

    prMeshData* testMesh = prMeshCreate();
    prMeshLink(testMesh, test->openglContext);
    prMeshUpdate(testMesh, vertices, 12, indices, 6, textureCoordinates, 8);

    prMeshData* testMesh2 = prMeshCreate();
    prMeshLink(testMesh2, test->openglContext);
    prMeshUpdate(testMesh2, vertices2, 12, indices, 6, textureCoordinates, 8);

    FILE* textureFile = fopen("awesomeface.png", "rb");
    fseek(textureFile, 0L, SEEK_END);
    size_t textureFileSize = ftell(textureFile);
    fseek(textureFile, 0L, SEEK_SET);
    unsigned char* textureData = prMalloc(textureFileSize + 1);
    fread(textureData, textureFileSize, 1, textureFile);
    fclose(textureFile);

    prTextureData* testTexture = prTextureCreate();
    prTextureLink(testTexture, test->openglContext);
    prTextureUpdate(testTexture, textureData, textureFileSize);
    prFree(textureData);

    int width, height;
    glfwGetWindowSize(test->window, &width, &height);
    test->openglContext->Viewport(0, 0, width, height);

    while(!glfwWindowShouldClose(test->window)) {
        test->openglContext->ClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        test->openglContext->Clear(GL_COLOR_BUFFER_BIT);

        prWindowDrawMesh(test, shaderProgram, testMesh, testTexture);
        prWindowDrawMesh(test, shaderProgram, testMesh2, testTexture);

        glfwSwapBuffers(test->window);

        glfwPollEvents();
    }

    prMeshDestroy(testMesh);
    testMesh = NULL;

    prTextureDestroy(testTexture);
    testTexture = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();
}
