#include <PR/PR.h>

#include <stdio.h>
#include <cglm/cglm.h>
#include <PR/memory.h>

float vertices[] = {
    0.6f,  0.6f, 0.0f, // top right
    0.6f, -0.6f, 0.0f, // bottom right
   -0.6f, -0.6f, 0.0f, // bottom left
   -0.6f,  0.6f, 0.0f  // top left
};

float vertices2[] = {
    1.0f,  1.0f, 0.0f, // top right
    1.0f, -0.0f, 0.0f, // bottom right
   -0.0f, -0.0f, 0.0f, // bottom left
   -0.0f,  1.0f, 0.0f  // top left
};

float vertices3[] = {
    0.0f,  0.0f, 0.0f, // top right
    0.0f, -1.0f, 0.0f, // bottom right
   -1.0f, -1.0f, 0.0f, // bottom left
   -1.0f,  0.0f, 0.0f  // top left
};

float textureCoordinates[] = {
    1.0f,  1.0f, // top right
    1.0f,  0.0f, // bottom right
    0.0f,  0.0f, // bottom left
    0.0f, 1.0f   // top left
};

float vertexColor[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
   0, 1, 3, // first Triangle
   1, 2, 3  // second Triangle
};

size_t g_width = 0;
size_t g_height = 0;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    GladGLContext* context = glfwGetWindowUserPointer(window);
    context->Viewport(0, 0, width, height);

    g_width = width;
    g_height = height;
}

int main(int argc, char** argv) {
    prSetLogLevel(PR_LOG_LEVEL_TRACE);
    prStartFileLogging();

    glfwInit();

    prWindow* test = prWindowCreate("Test", 600, 600);
    prWindowInitContext(test);
    glfwSetWindowUserPointer(test->window, test->openglContext);

    prEnableBlending(test->openglContext);
    test->openglContext->Enable(GL_DEPTH_TEST);
    prEnableImageFlip();

    unsigned int shaderProgram = prShaderGenerateDefaultProgram(test->openglContext);

    FILE* textureFile = fopen("res/awesomeface.png", "rb");
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

    textureFile = fopen("res/container.jpg", "rb");
    fseek(textureFile, 0L, SEEK_END);
    textureFileSize = ftell(textureFile);
    fseek(textureFile, 0L, SEEK_SET);
    textureData = prMalloc(textureFileSize + 1);
    fread(textureData, textureFileSize, 1, textureFile);
    fclose(textureFile);

    prTextureData* testTexture2 = prTextureCreate();
    prTextureLink(testTexture2, test->openglContext);
    prTextureUpdate(testTexture2, textureData, textureFileSize);
    prFree(textureData);

    prMeshData* testMesh = prMeshCreate();
    prMeshLinkWindow(testMesh, test->openglContext);
    prMeshLinkTexture(testMesh, testTexture);
    prMeshUpdate(testMesh, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMesh, 0.5f);

    prMeshData* testMesh2 = prMeshCreate();
    prMeshLinkWindow(testMesh2, test->openglContext);
    prMeshLinkTexture(testMesh2, testTexture2);
    prMeshUpdate(testMesh2, vertices2, sizeof(vertices2) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), textureCoordinates, sizeof(textureCoordinates) / sizeof(float), NULL, 0);
    prMeshTextureToColorRatio(testMesh2, 1.0f);

    prMeshData* testMesh3 = prMeshCreate();
    prMeshLinkWindow(testMesh3, test->openglContext);
    prMeshUpdate(testMesh3, vertices3, sizeof(vertices3) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int), NULL, 0, vertexColor, sizeof(vertexColor) / sizeof(float));
    prMeshTextureToColorRatio(testMesh3, 0.0f);

    glfwMakeContextCurrent(test->window);
    glfwSetFramebufferSizeCallback(test->window, framebufferSizeCallback);

    while(!glfwWindowShouldClose(test->window)) {
        prWindowClear(test->openglContext);

        mat4 translation;
        glm_mat4_identity(translation);
        vec3 throwAwayVector;
        throwAwayVector[0] = 1.0f;
        throwAwayVector[1] = 0.0f;
        throwAwayVector[2] = 0.0f;
        glm_rotate(translation, -120.0f, throwAwayVector);

        mat4 view;
        glm_mat4_identity(view);
        throwAwayVector[0] = 0.0f;
        throwAwayVector[1] = 0.0f;
        throwAwayVector[2] = -3.0f;
        glm_translate(view, throwAwayVector);

        mat4 projection;
        glm_perspective(45.0f, (float)g_width/(float)g_height, 0.1f, 100.0f, projection);

        int viewUniformLocation = test->openglContext->GetUniformLocation(shaderProgram, "view");
        test->openglContext->UniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, view[0]);

        int projectionUniformLocation = test->openglContext->GetUniformLocation(shaderProgram, "projection");
        test->openglContext->UniformMatrix4fv(projectionUniformLocation, 1, GL_FALSE, projection[0]);

        prMeshDraw(testMesh2, translation, shaderProgram);
        prMeshDraw(testMesh3, translation, shaderProgram);
        prMeshDraw(testMesh, translation, shaderProgram);

        glfwSwapBuffers(test->window);

        glfwPollEvents();
    }

    prMeshDestroy(testMesh);
    testMesh = NULL;

    prTextureDestroy(testTexture);
    testTexture = NULL;
    prTextureDestroy(testTexture2);
    testTexture2 = NULL;

    prWindowDestroy(test);
    test = NULL;

    glfwTerminate();

    prEndFileLogging();
}
