#include <PR/PR.h>

#include <stdio.h>
#include <PR/memory.h>

float vertices[] = {
    0.8f,  0.8f, 0.0f, // top right
    0.8f, -0.8f, 0.0f, // bottom right
   -0.8f, -0.8f, 0.0f, // bottom left
   -0.8f,  0.8f, 0.0f  // top left
};

float vertices2[] = {
    1.0f,  1.0f, 0.0f, // top right
    1.0f, -0.2f, 0.0f, // bottom right
   -0.2f, -0.2f, 0.0f, // bottom left
   -0.2f,  1.0f, 0.0f  // top left
};

float textureCoordinates[] = {
    1.0f,  1.0f, // top right
    1.0f,  0.0f, // bottom right
    0.0f,  0.0f, // bottom left
    0.0f, 1.0f   // top left
};
unsigned int indices[] = {
   0, 1, 3, // first Triangle
   1, 2, 3  // second Triangle
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    prSetLogLevel(PR_LOG_LEVEL_TRACE);
    prStartFileLogging();

    glfwInit();

    prWindow* test = prWindowCreate("Test", 600, 600);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);
    
    prEnableBlending(test->openglContext);
    prEnableImageFlip();
    
    unsigned int shaderProgram = prShaderGenerateDefaultProgram(test->openglContext);
    
    prMeshData* testMesh = prMeshCreate();
    prMeshLink(testMesh, test->openglContext);
    prMeshUpdate(testMesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float));
    
    prMeshData* testMesh2 = prMeshCreate();
    prMeshLink(testMesh2, test->openglContext);
    prMeshUpdate(testMesh2, vertices2, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float));
    
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
    
    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);

    while(!glfwWindowShouldClose(test->window)) {
        prWindowClear(test->openglContext);

        prWindowDrawMesh(test->openglContext, shaderProgram, testMesh, testTexture);
        prWindowDrawMesh(test->openglContext, shaderProgram, testMesh2, testTexture);

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

    prEndFileLogging();
}
